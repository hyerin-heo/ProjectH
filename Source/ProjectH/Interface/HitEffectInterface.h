// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitEffectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitEffectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTH_API IHitEffectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayEffect(UWorld* InWorld, const FVector& HitLocation, const FVector& HitNormal, const FVector& EffectScale, AActor* DamageCauser) = 0;
};
