// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "Interface/HitEffectInterface.h"
#include "NiagaraHitEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UNiagaraHitEffectComponent : public UActorComponent, public IHitEffectInterface
{
	GENERATED_BODY()

public:    
	UNiagaraHitEffectComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	UNiagaraSystem* NiagaraFXToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect)
	uint8 bShouldDealDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (EditCondition = "bShouldDealDamage"))
	float DamageRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (EditCondition = "bShouldDealDamage"))
	float DamageAmount;

public:    
	virtual void PlayEffect(UWorld* InWorld, const FVector& HitLocation, const FVector& HitNormal, const FVector& EffectScale, AActor* DamageCauser) override;
};
