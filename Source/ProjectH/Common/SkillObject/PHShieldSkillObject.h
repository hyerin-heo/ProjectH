// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "PHShieldSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHShieldSkillObject : public ASkillObjectBase
{
	GENERATED_BODY()
	
public:
	APHShieldSkillObject();

	virtual void BeginPlay() override;
	
	virtual void ResetProjectile() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit) override;

	virtual void Client_ResetProjectile_Implementation() override;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = SkillObject)
	UNiagaraComponent* NiagaraComponent;
	
protected:
	
};
