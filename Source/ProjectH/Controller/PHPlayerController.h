// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APHPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
};
