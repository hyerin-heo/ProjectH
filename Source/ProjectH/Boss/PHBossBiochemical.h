// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "Common/SkillObjectBase.h"
#include "PHBossBiochemical.generated.h"

UENUM(BlueprintType)
enum class SkillObjectType : uint8
{
	SwordAura,
};

/**
 * 
 */
UCLASS()
class PROJECTH_API APHBossBiochemical : public APHBossCharacterBase
{
	GENERATED_BODY()
public:
	APHBossBiochemical();
	virtual void BeginPlay() override;
	
	virtual void AttackAction() override;
	
	void Pattern1();
	void Pattern2();
	void Pattern3();
	void Pattern4();
	void Pattern5();
	
	void Pattern1HitCheck(const FBossPatternInfo& PatternInfo);
	void Pattern2HitCheck(const FBossPatternInfo& PatternInfo);
	void Pattern3HitCheck(const FBossPatternInfo& PatternInfo);
	void Pattern4HitCheck(const FBossPatternInfo& PatternInfo);
	void Pattern5HitCheck(const FBossPatternInfo& PatternInfo);
	void Test(float Damage);

	virtual void PatternHitCheck(const int32& InPatternIndex, const uint8& InStep) override;

protected:
	virtual void PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase) override;

	UPROPERTY(EditAnywhere)
	TMap<SkillObjectType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;
};
