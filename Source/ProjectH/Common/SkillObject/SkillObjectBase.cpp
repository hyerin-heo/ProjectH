// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/SkillObjectBase.h"

#include "Common/Common.h"
#include "Common/HitObject/PHHitEffectActor.h"
#include "Engine/DamageEvents.h"
#include "Physics/PHCollision.h"

// Sets default values
ASkillObjectBase::ASkillObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
	// 복제 주기
	NetUpdateFrequency = 30.0f;
	MinNetUpdateFrequency = 5.0f;

	// Collision setting
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SphereComp"));
	CollisionComponent->InitCapsuleSize(5.f, 10.f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(CPROFILE_TRIGGER);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASkillObjectBase::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASkillObjectBase::OnOverlapEnd);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASkillObjectBase::OnHit);//바닥 충돌은 Hit로 처리.

	// Unwalkable
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;
	
	// Default
	Damage = 10.f;

	InitialLifeSpan = 0.0f;

	CurrentHitType = ESkillObjectHitType::NormalHit;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void ASkillObjectBase::BeginPlay()
{
	Super::BeginPlay();
	LifeSpanDeltaTime = 0.f;
	LifeSpan = 0.f;
	// if (!HasAuthority())
	{
		PH_LOG(LogPHGameFlow, Warning, TEXT("SpawnSkillObject Name: %s"), *GetName());	
	}
}

void ASkillObjectBase::PostNetInit()
{
	Super::PostNetInit();
	PH_LOG(LogPHGameFlow, Warning, TEXT("Name : %s"), *GetName());
}

void ASkillObjectBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (CurrentHitType != ESkillObjectHitType::ResetOnWorldStaticHit)
		{
			return;
		}
		if (OtherComp && OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
		{
			if (bReturnToPoolOnHit)
			{
				ResetProjectile();
			}
			HitOnWorld(Hit);
		}
	}
}

void ASkillObjectBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (!Other)
		{
			return;
		}
		if ((Other->ActorHasTag(TAG_ALLY) && this->Owner->ActorHasTag(TAG_ALLY)) || (Other->ActorHasTag(TAG_ENEMY) && this->Owner->ActorHasTag(TAG_ENEMY)))
		{
			//같은편이 쏜거임.
			return;
		}
		
		if ((Other != nullptr) && (Other != this) && Other != this->Owner && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		switch (CurrentHitType) {
		case ESkillObjectHitType::ResetOnWorldStaticHit:
		case ESkillObjectHitType::NormalHit:
			{
				// Damage
				if (Other && Other != this && Other != this->Owner)
				{
					// UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
					FDamageEvent DamageEvent;
					Other->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
					HitOnOpponent(SweepResult);
				}
				
				if (bReturnToPoolOnHit && CurrentHitType != ESkillObjectHitType::ResetOnWorldStaticHit)
				{
					ResetProjectile();
				}
			}
			break;
		case ESkillObjectHitType::TickDamage:
			{
				// 틱 데미지 로직 (시나리오 2에서 구현한 것과 동일)
				if (!ActiveTickDamageTargets.Contains(Other))
				{
					FTimerHandle TimerHandle;
					GetWorldTimerManager().SetTimer(TimerHandle, [&]()
					{
						DealTickDamage(Other, Damage);
					}, TickDamageInterval, true);
					ActiveTickDamageTargets.Add(Other, TimerHandle);
				}
				FDamageEvent InitialDamageEvent;
				Other->TakeDamage(Damage, InitialDamageEvent, GetInstigatorController(), this);
				HitOnOpponent(SweepResult);
			}
			break;
		}
	}
}

void ASkillObjectBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetLocalRole() == ROLE_Authority && CurrentHitType == ESkillObjectHitType::TickDamage)
	{
		if (!OtherActor)
		{
			return;
		}
		if ((OtherActor->ActorHasTag(TAG_ALLY) && this->Owner->ActorHasTag(TAG_ALLY)) || (OtherActor->ActorHasTag(TAG_ENEMY) && this->Owner->ActorHasTag(TAG_ENEMY)))
		{
			//같은편이 쏜거임.
			return;
		}
		if (CurrentHitType == ESkillObjectHitType::TickDamage)
		{
			if (ActiveTickDamageTargets.Contains(OtherActor))
			{
				GetWorldTimerManager().ClearTimer(ActiveTickDamageTargets[OtherActor]);
				ActiveTickDamageTargets.Remove(OtherActor);
			}
		}
	}
}

// Called every frame
void ASkillObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LifeSpan > 0)
	{
		LifeSpanDeltaTime += DeltaTime;
		if (LifeSpanDeltaTime >= LifeSpan)
		{
			ResetProjectile();
		}
	}
}

void ASkillObjectBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!CollisionComponent->IsRenderStateCreated())
	{
		CollisionComponent->UnregisterComponent();
		CollisionComponent->RegisterComponent();
	}
}

void ASkillObjectBase::ResetProjectile()
{
	// must be called only server.
	// replicated at client.
	
	// disable
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	// reset lifetime -> 소멸 방지
	// SetLifeSpan(0.0f);
	LifeSpan = 0.f;
	LifeSpanDeltaTime = 0.f;
	if (CurrentHitType == ESkillObjectHitType::TickDamage)
	{
		for (auto& Pair : ActiveTickDamageTargets)
		{
			GetWorldTimerManager().ClearTimer(Pair.Value);
		}
		ActiveTickDamageTargets.Empty();
	}
	Client_ResetProjectile();
}

void ASkillObjectBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void ASkillObjectBase::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
	FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	// 클라이언트에서 메시를 확실히 보이게 하고 이동 상태 설정
	SetActorLocation(InLocation);
	SetActorRotation(InRotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = bInReturnToPoolOnHit;
	Damage = InDamage;
}

void ASkillObjectBase::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	// 클라이언트에서 액터 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	LifeSpan = 0.f;
	LifeSpanDeltaTime = 0.f;
}

void ASkillObjectBase::HitOnWorld(const FHitResult& HitResult)
{
	if (HitWorldEffectComponentClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;
        
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(HitWorldEffectComponentClass, HitResult.ImpactPoint, FRotator::ZeroRotator, SpawnParams);
		SpawnedActor->SetLifeSpan(3.0f);
	}
}

void ASkillObjectBase::HitOnOpponent(const FHitResult& HitResult)
{
	if (HitEffectComponentClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;
        
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(HitEffectComponentClass, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation(), SpawnParams);
		SpawnedActor->SetLifeSpan(1.0f); 
	}
}

void ASkillObjectBase::DealTickDamage(AActor* TargetActor, float TickDamageAmount)
{
	if (IsValid(TargetActor) && ActiveTickDamageTargets.Contains(TargetActor)) // 대상이 유효하고 아직 틱 데미지 중이라면
	{
		FDamageEvent DamageEvent;
		TargetActor->TakeDamage(TickDamageAmount, DamageEvent, GetInstigatorController(), this);
		PH_LOG(LogTemp, Log, TEXT("%s took %f tick damage from %s"), *TargetActor->GetName(), TickDamageAmount, *GetName());
	}
	else // 대상이 유효하지 않거나 틱 데미지가 중지되었다면 타이머 해제
	{
		if (ActiveTickDamageTargets.Contains(TargetActor))
		{
			GetWorldTimerManager().ClearTimer(ActiveTickDamageTargets[TargetActor]);
			ActiveTickDamageTargets.Remove(TargetActor);
		}
	}
}
