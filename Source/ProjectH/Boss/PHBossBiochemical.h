// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "PHBossBiochemical.generated.h"

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

protected:
	virtual void PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase) override;
};
