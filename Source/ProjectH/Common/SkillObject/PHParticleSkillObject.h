// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "PHParticleSkillObject.generated.h"

class UParticleSystemComponent;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHParticleSkillObject : public ASkillObjectBase
{
	GENERATED_BODY()
public:
	APHParticleSkillObject();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Launch(const FVector& Direction, float InDamage) override;

	virtual void Launch(float InDamage, float InLifeTime) override;

	virtual void Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit) override;

	virtual void Client_ResetProjectile_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects)
	TObjectPtr<UParticleSystemComponent> ParticleComponent;
};
