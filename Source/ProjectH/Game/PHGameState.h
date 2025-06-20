// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHPlayerState.h"
#include "Common/GlobalEnum.h"
#include "GameFramework/GameState.h"
#include "PHGameState.generated.h"

/**
 * 
 */

USTRUCT()
struct FSelectedClassInfo
{
	GENERATED_BODY()

public:
	FSelectedClassInfo() {}
	FSelectedClassInfo(EClassType InClassType, APHPlayerState* InPlayerState)
		: ClassType(InClassType), PlayerState(InPlayerState)
	{
		
	}
	

	UPROPERTY()
	EClassType ClassType;

	UPROPERTY()
	APHPlayerState* PlayerState;
};


UCLASS()
class PROJECTH_API APHGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void AddSelectedClassArray(EClassType Class, APHPlayerState* PlayerState);
	bool IsClassReadySelected(EClassType Class) const;

	FORCEINLINE void GameStart(){bGameStart = true;}

	UFUNCTION()
	void OnRep_SelectedClassArray();

	UFUNCTION()
	void OnRep_bGameStart();


private:
	UPROPERTY(ReplicatedUsing = OnRep_SelectedClassArray)
	TArray<FSelectedClassInfo> SelectedClassArray;

	UPROPERTY(ReplicatedUsing = OnRep_bGameStart)
	uint8 bGameStart:1 = false;
	
	
};
