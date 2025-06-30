// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/SkillObject/PHMeshSkillObject.h"

APHMeshSkillObject::APHMeshSkillObject()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent->SetIsReplicated(true);
}

void APHMeshSkillObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!MeshComponent->IsRenderStateCreated())
	{
		MeshComponent->UnregisterComponent();
		MeshComponent->RegisterComponent();
	}
}

void APHMeshSkillObject::BeginPlay()
{
	Super::BeginPlay();
}

void APHMeshSkillObject::Launch(const FVector& Direction, float InDamage)
{
	Super::Launch(Direction, InDamage);
}

void APHMeshSkillObject::Launch(float InDamage, float InLifeTime)
{
	Super::Launch(InDamage, InLifeTime);
}

void APHMeshSkillObject::Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation,
	FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit)
{
	Super::Client_ActivateSkillObject_Implementation(InLocation, InRotation, InVelocity, InDamage, InLifeTime,
	                                                 bInReturnToPoolOnHit);
}

void APHMeshSkillObject::Client_ResetProjectile_Implementation()
{
	Super::Client_ResetProjectile_Implementation();
}
