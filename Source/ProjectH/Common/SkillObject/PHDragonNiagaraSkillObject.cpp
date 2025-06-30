// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHDragonNiagaraSkillObject.h"

#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/PHSoundManager.h"

APHDragonNiagaraSkillObject::APHDragonNiagaraSkillObject()
{
	NiagaraComponent2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraFX2"));
	NiagaraComponent2->SetupAttachment(RootComponent);

	// 자동 재생 비활성화
	NiagaraComponent2->bAutoActivate = false;

	NiagaraComponent2->SetIsReplicated(true);
	SetActorEnableCollision(false);
	//SetReplicates(true);
	bReplicates = true;
	
}

void APHDragonNiagaraSkillObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!NiagaraComponent2->IsRenderStateCreated())
	{
		NiagaraComponent2->UnregisterComponent();
		NiagaraComponent2->RegisterComponent();
	}
}

void APHDragonNiagaraSkillObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHDragonNiagaraSkillObject, EnableCollisionTime);
}

void APHDragonNiagaraSkillObject::BeginPlay()
{
	Super::BeginPlay();

	SetActorEnableCollision(false);
	
}

void APHDragonNiagaraSkillObject::Launch(const FVector& Direction, float InDamage)
{
	if (NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
	Super::Launch(Direction, InDamage);

}

void APHDragonNiagaraSkillObject::Launch(float InDamage, float InLifeTime)
{
	if (NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
	Super::Launch(InDamage, InLifeTime);

}

void APHDragonNiagaraSkillObject::Launch(float InDamage, float InLifeTime, float InEnableCollisionTime)
{
	SetActorEnableCollision(false);
	
	EnableCollisionTime = InEnableCollisionTime;

	Init(InLifeTime);
	SetActorTickEnabled(true);
	Damage = InDamage;
	// 클라이언트에 액터 활성화 및 초기 이동 상태를 직접 알려주는 RPC 호출

	if (NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
	NiagaraComponent->Activate(true);

	if (EnableCollisionTime > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			SetActorEnableCollision(true);
			SetPlaySFX();
		}), EnableCollisionTime, false);	
	}

	Client_ActivateSkillObject(GetActorLocation(), GetActorRotation(), MovementComponent->Velocity, InDamage, LifeSpan, bReturnToPoolOnHit);
}

void APHDragonNiagaraSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}

	SetActorEnableCollision(false);

	NiagaraComponent->Activate(true);
	if (NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}

	// 클라이언트에서 메시를 확실히 보이게 하고 이동 상태 설정
	SetActorLocation(InLocation);
	SetActorRotation(InRotation);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	if (EnableCollisionTime > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			SetActorEnableCollision(true);
			SetPlaySFX();
		}), EnableCollisionTime, false);	
	}

	LifeSpan = InLifeTime;
	LifeSpanDeltaTime = 0.f;
	bReturnToPoolOnHit = bInReturnToPoolOnHit;
	Damage = InDamage;
	
	// 이동 컴포넌트 상태 동기화
	MovementComponent->InitialSpeed = InVelocity.Size();
	MovementComponent->MaxSpeed = InVelocity.Size() * 2.0f;
	MovementComponent->Velocity = InVelocity;
	MovementComponent->Activate(); // 이동 시작
	
}

void APHDragonNiagaraSkillObject::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	
	Super::Client_ResetProjectile_Implementation();
	
	if (NiagaraComponent2)
	{
		NiagaraComponent2->Deactivate();
	}
	SetActorEnableCollision(false);
}

void APHDragonNiagaraSkillObject::SetPlaySFX()
{
	if (SkillNumber == 2)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
				{
					SoundManager->PlaySFX(ESoundCategory::SFX, TEXT("Explosion_Space_2"), 0.7f);
				}
			}
		}
	}
	else if (SkillNumber == 3)
	{
		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
				{
					SoundManager->PlaySFX(ESoundCategory::SFX, TEXT("Cannon_shots_7"), 0.7f);
				}
			}
		}
	}
}
