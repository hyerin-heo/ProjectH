// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHShieldSkillObject.h"

APHShieldSkillObject::APHShieldSkillObject()
{
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraFX"));
	NiagaraComponent->SetupAttachment(RootComponent);

	// 자동 재생 비활성화
	NiagaraComponent->bAutoActivate = false;
}

void APHShieldSkillObject::BeginPlay()
{
	Super::BeginPlay();
}

void APHShieldSkillObject::ResetProjectile()
{
	Super::ResetProjectile();
}

void APHShieldSkillObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APHShieldSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
	FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime,
	                                                 bInReturnToPoolOnHit);
}

void APHShieldSkillObject::Client_ResetProjectile_Implementation()
{
	Super::Client_ResetProjectile_Implementation();
}
