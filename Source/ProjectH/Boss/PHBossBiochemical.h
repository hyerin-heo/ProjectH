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
	
	void Attack1();
	void Attack2();
	void Attack3();
	
	void SpecialAttack1();
	void SpecialAttack2();
	void SpecialAttack3();
};
