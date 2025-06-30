// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHParticleSkillObject.h"

#include "Particles/ParticleSystemComponent.h"

APHParticleSkillObject::APHParticleSkillObject()
{
	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleFX"));
	ParticleComponent->SetupAttachment(RootComponent);
	ParticleComponent->bAutoActivate = false;
	ParticleComponent->SetIsReplicated(true);
}

void APHParticleSkillObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!ParticleComponent->IsRenderStateCreated())
	{
		ParticleComponent->UnregisterComponent();
		ParticleComponent->RegisterComponent();
	}
}

void APHParticleSkillObject::BeginPlay()
{
	Super::BeginPlay();
}

void APHParticleSkillObject::Launch(const FVector& Direction, float InDamage)
{
	Super::Launch(Direction, InDamage);
	ParticleComponent->Activate(true);
}

void APHParticleSkillObject::Launch(float InDamage, float InLifeTime)
{
	Super::Launch(InDamage, InLifeTime);
	ParticleComponent->Activate(true);
}

void APHParticleSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
	FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime,
	                                                 bInReturnToPoolOnHit);
	ParticleComponent->Activate(true);
}

void APHParticleSkillObject::Client_ResetProjectile_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		return;
	}
	Super::Client_ResetProjectile_Implementation();
	ParticleComponent->Deactivate();
}
