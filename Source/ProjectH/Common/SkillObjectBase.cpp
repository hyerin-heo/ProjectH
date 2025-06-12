// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObjectBase.h"

#include "ProjectH.h"
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
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASkillObjectBase::OnHit);

	// Unwalkable
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	Mesh->SetVisibility(true);
	Mesh->SetupAttachment(RootComponent);
	
	// Default
	InitialSpeed = 3000.f;
	MaxSpeed = 3000.f;
	Damage = 10.f;

	// Init ProjectileMovementComponent
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 3000.f;
	MovementComponent->MaxSpeed = 3000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->SetIsReplicated(true); 

	InitialLifeSpan = 0.0f;
}

// Called when the game starts or when spawned
void ASkillObjectBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	MovementComponent->InitialSpeed = InitialSpeed;
	MovementComponent->MaxSpeed = MaxSpeed;
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
	LifeSpanDeltaTime = 0.f;
	LifeSpan = 0.f;
	Mesh->Activate();
}

void ASkillObjectBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		// Damage
		if (OtherActor && OtherActor != this)
		{
			// UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
			FDamageEvent DamageEvent;
			OtherActor->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
		}

		// For recycle
		if (bReturnToPoolOnHit)
		{
			LifeSpanExpired();
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

void ASkillObjectBase::Launch(const FVector& Direction, float InDamage)
{
	
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	Damage = InDamage;
	MovementComponent->Velocity = Direction * InitialSpeed;
	MovementComponent->Activate();
}

void ASkillObjectBase::Launch(float InDamage, float InLifeTime)
{
	Init(InLifeTime);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	Damage = InDamage;
}

void ASkillObjectBase::Init(float InSpeed, float InLifeTime, bool ReturnToPoolOnHit)
{
	MovementComponent->InitialSpeed = InSpeed;
	MovementComponent->MaxSpeed = InSpeed * 2.0f;
	
	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = ReturnToPoolOnHit;
}

void ASkillObjectBase::Init(const FVector& InEndLocation, float InSpeed, float InLifeTime, bool ReturnToPoolOnHit)
{
	FVector StartLocation = GetActorLocation();
	float Distance = FVector::Dist(StartLocation, InEndLocation);
	float Speed = InSpeed;
	LifeSpan = InLifeTime;
	if (Speed > KINDA_SMALL_NUMBER)
	{
		LifeSpan = Distance / Speed;
	}
	else if (LifeSpan > KINDA_SMALL_NUMBER)
	{
		Speed = Distance / LifeSpan;
	}
	else
	{
		LifeSpan = 3.0f;
		Speed = Distance / LifeSpan;
	}
	MovementComponent->InitialSpeed = Speed;
	MovementComponent->MaxSpeed = Speed * 2.0f;
	bReturnToPoolOnHit = ReturnToPoolOnHit;

	LifeSpanDeltaTime = 0.f;
}

void ASkillObjectBase::Init(float InLifeTime)
{
	MovementComponent->InitialSpeed = 0.0f;
	MovementComponent->MaxSpeed = 0.0f;
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
	
	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = false;
}

void ASkillObjectBase::ResetProjectile()
{
	// must be called only server.
	// replicated at client.
	
	// disable
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	MovementComponent->StopMovementImmediately();
	MovementComponent->Deactivate();
	// reset lifetime -> 소멸 방지
	// SetLifeSpan(0.0f);
	LifeSpan = 0.f;
	LifeSpanDeltaTime = 0.f;
}

void ASkillObjectBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME(AMyProjectile, ProjectileMovement);
}

void ASkillObjectBase::LifeSpanExpired()
{
	// Do not destroy
	// Super::LifeSpanExpired();
	ResetProjectile();
}

void ASkillObjectBase::Destroyed()
{
	Super::Destroyed();
	PH_LOG(LogPHBoss, Error, TEXT("Destroy!!"));
}

void ASkillObjectBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	PH_LOG(LogPHBoss, Error, TEXT("Destroy!!"));
}

