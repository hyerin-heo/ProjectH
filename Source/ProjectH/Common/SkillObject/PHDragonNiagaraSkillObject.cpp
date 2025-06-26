// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHDragonNiagaraSkillObject.h"

#include "NiagaraComponent.h"

APHDragonNiagaraSkillObject::APHDragonNiagaraSkillObject()
{
	NiagaraComponent2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraFX2"));
	NiagaraComponent2->SetupAttachment(RootComponent);

	// 자동 재생 비활성화
	NiagaraComponent2->bAutoActivate = false;

	SetActorEnableCollision(false);
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

void APHDragonNiagaraSkillObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APHDragonNiagaraSkillObject::Launch(const FVector& Direction, float InDamage)
{
	Super::Launch(Direction, InDamage);

	if (!NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
}

void APHDragonNiagaraSkillObject::Launch(float InDamage, float InLifeTime)
{
	Super::Launch(InDamage, InLifeTime);

	if (!NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
}

void APHDragonNiagaraSkillObject::Launch(float InDamage, float InLifeTime, float InEnableCollisionTime)
{
	Super::Launch(InDamage, InLifeTime);

	if (!NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}

	if (InEnableCollisionTime > 0.0f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]
		{
			SetActorEnableCollision(true);
		}), InEnableCollisionTime, false);	
	}
}

void APHDragonNiagaraSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
                                                                            FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime,bInReturnToPoolOnHit);

	if (!NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
	
}

void APHDragonNiagaraSkillObject::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ResetProjectile_Implementation();
	if (!NiagaraComponent2)
	{
		NiagaraComponent2->Activate(true);
	}
	SetActorEnableCollision(false);
}
