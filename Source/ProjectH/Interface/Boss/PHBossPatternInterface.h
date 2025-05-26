// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHBossPatternInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHBossPatternInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTH_API IPHBossPatternInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void CommonPattern() = 0;
	virtual void SpecialPattern() = 0;
	virtual void SetPhase(uint8 level) = 0;
};
