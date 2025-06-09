// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PHBossAIInterface.generated.h"

DECLARE_DELEGATE(FAIAttackFinished);
DECLARE_DELEGATE(FAIPatternAttackFinished);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPHBossAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTH_API IPHBossAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetDetectionRadius() = 0;
	virtual float GetAttackRange() = 0;
	virtual float GetSpeed() = 0;
	virtual float GetAttackSpeed() = 0;
	virtual float GetArmor() = 0;

	virtual bool IsPhase() = 0;
	
	virtual void AttackAction() = 0;

	virtual void PatternAction() = 0;
	virtual void PhasePatternAction() = 0;
	
	virtual void AttackActionRPC() = 0;
	
	virtual void SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished) = 0;
	virtual void SetAIPatternAttackDelegate(const FAIPatternAttackFinished& InOnPatternAttackFinished) = 0;

	virtual bool IsCoolTime() = 0;
};
