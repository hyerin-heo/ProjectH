// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PHCharacterStatDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None = 0,
	DefaultAttack,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
};

UENUM(BlueprintType)
enum class EAttackStatType : uint8
{
	None = 0,
	Damage,
	Heal,
	Armor
};

USTRUCT(BlueprintType)
struct FCharacterAttackStat
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackStatType AttackStatType = EAttackStatType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float minPercent = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxPercent = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoolTime = 1.0f;
};

UCLASS()
class PROJECTH_API UPHCharacterStatDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Stat)
	float MaxHp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackDamageStat)
	TMap<EAttackType,FCharacterAttackStat> AttackStatMap;
	
};
