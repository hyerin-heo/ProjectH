// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SkillObject/PHNiagaraSkillObject.h"
#include "PHDragonNiagaraSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHDragonNiagaraSkillObject : public APHNiagaraSkillObject
{
	GENERATED_BODY()


public:
	APHDragonNiagaraSkillObject();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Launch(const FVector& Direction, float InDamage) override;

	virtual void Launch(float InDamage, float InLifeTime) override;

	void Launch(float InDamage, float InLifeTime, float InEnableCollisionTime = 0.0f);

	virtual void Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit) override;

	virtual void Client_ResetProjectile_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects)
	TObjectPtr<UNiagaraComponent> NiagaraComponent2;
};
