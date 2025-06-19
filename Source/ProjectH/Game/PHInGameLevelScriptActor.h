// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "PHInGameLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHInGameLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	bool LoadStageLevel(int32 Index);
	
	UFUNCTION(NetMulticast, Reliable)
	void LoadStageLevelRPC(int32 Index);

	UFUNCTION(Exec)
	void UnLoadStageLevel(int32 Index);
	
	UFUNCTION(NetMulticast, Reliable)
	void UnLoadStageLevelRPC(int32 Index);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnStreamLevelLoaded();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	TArray<FName> StageLevelNames;

private:
	int32 LoadedStreamingLevelsCount;
};
