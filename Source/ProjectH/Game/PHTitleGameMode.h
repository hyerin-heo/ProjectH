// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PHTitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHTitleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APHTitleGameMode();
	
	void JoinGame();

protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
