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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Launch(const FVector& Direction, float InDamage) override;

	virtual void Launch(float InDamage, float InLifeTime) override;

	virtual void Launch(float InDamage, float InLifeTime, float InEnableCollisionTime) override;

	virtual void Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit) override;

	virtual void Client_ResetProjectile_Implementation() override;

	void SetPlaySFX();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effects)
	TObjectPtr<UNiagaraComponent> NiagaraComponent2;

	UPROPERTY(Replicated)
	float EnableCollisionTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillNum, meta = (AllowPrivateAccess = "true"))
	uint8 SkillNumber = 0;
};
