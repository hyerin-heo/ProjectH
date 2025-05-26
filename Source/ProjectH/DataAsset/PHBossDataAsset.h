// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHBossDataAsset.generated.h"
UENUM(BlueprintType)
enum class EBossPhaseTriggerType : uint8
{
	HealthPercent,
	Timer
};


USTRUCT(BlueprintType)
struct FBossPhaseInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBossPhaseTriggerType TriggerType = EBossPhaseTriggerType::HealthPercent;

	// 1.0 => 100%
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TriggerValue = 1.0f;
};

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHBossDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Phase")
	TMap<uint8, FBossPhaseInfo> PhaseMap;

	
	
};
