// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHCharacterShieldInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHCharacterShieldInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTH_API IPHCharacterShieldInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsShieldActive() const = 0;
};
