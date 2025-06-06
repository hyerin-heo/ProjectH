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

USTRUCT(BlueprintType)
struct FBossPatternInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PatternIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;

	// Which used animation's speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed;

	// // 0 ~ 1
	// // 0 -> default Armor. 1 -> invincibility
	// // Reduces the percentage of damage which taken boss
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// float Armor;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoolTime;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float DetectionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float AttackSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Common)
	float Armor;
	// // All of pattern's cool time is same
	// // Used when not set pattern info's cool time.
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pattern)
	// float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Phase)
	TMap<uint8, FBossPhaseInfo> PhaseMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Pattern)
	TMap<uint8, FBossPatternInfo> Patterns;
	
};
