// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "PHBossBiochemical.generated.h"

UENUM(BlueprintType)
enum class ESkillObjectType : uint8
{
	GreenSwordAura,
	DarkGreenSwordAura,
	DarkRedSwordAura,
	StoneRush,
	DashMagma,
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
	
	void Pattern1HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step);
	void Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step);
	void Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step);
	void Pattern4HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step);

	virtual void PatternHitCheck(const int32& InPatternIndex, const uint8& InStep) override;

protected:
	virtual void PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase) override;

	UPROPERTY(EditAnywhere)
	TMap<ESkillObjectType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;
};
