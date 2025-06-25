// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "PHHitEffectActor.generated.h"

UCLASS()
class PROJECTH_API APHHitEffectActor : public AActor
{
	GENERATED_BODY()

public:    
	APHHitEffectActor();

	virtual void BeginPlay() override;

protected:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	// uint8 bIsUseNiagaraEffect:1;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (editcondition = "bIsUseNiagaraEffect"))
	// UNiagaraComponent* NiagaraFXToPlay;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (editcondition = "bIsUseNiagaraEffect == false"))
	// UParticleSystemComponent* ParticleFXToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	uint8 bShouldDealDamage:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (EditCondition = "bShouldDealDamage"))
	float DamageRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (EditCondition = "bShouldDealDamage"))
	float DamageAmount;
};
