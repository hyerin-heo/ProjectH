// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHNiagaraSkillObject.h"

#include "NiagaraComponent.h"

APHNiagaraSkillObject::APHNiagaraSkillObject()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraFX"));

	NiagaraComponent->SetupAttachment(RootComponent);

	// 자동 재생 비활성화
	NiagaraComponent->bAutoActivate = false;
}

void APHNiagaraSkillObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!NiagaraComponent->IsRenderStateCreated())
	{
		NiagaraComponent->UnregisterComponent();
		NiagaraComponent->RegisterComponent();
	}
}

void APHNiagaraSkillObject::BeginPlay()
{
	Super::BeginPlay();
}

void APHNiagaraSkillObject::Launch(const FVector& Direction, float InDamage)
{
	Super::Launch(Direction, InDamage);
	NiagaraComponent->Activate(true);
	
}

void APHNiagaraSkillObject::Launch(float InDamage, float InLifeTime)
{
	Super::Launch(InDamage, InLifeTime);
	NiagaraComponent->Activate(true);
}

void APHNiagaraSkillObject::Launch(float InDamage, float InLifeTime, float InEnableCollisionTime)
{
	Super::Launch(InDamage, InLifeTime, InEnableCollisionTime);
	NiagaraComponent->Activate(true);
}

void APHNiagaraSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity,
                                                                      float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime, bInReturnToPoolOnHit);
	NiagaraComponent->Activate(true);
}

void APHNiagaraSkillObject::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ResetProjectile_Implementation();
	NiagaraComponent->Deactivate();
}
