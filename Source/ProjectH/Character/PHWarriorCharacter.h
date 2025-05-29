// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHWarriorCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHWarriorCharacter : public APHCharacterBase
{
	GENERATED_BODY()

public:
	APHWarriorCharacter(const FObjectInitializer& ObjectInitializer);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	//Attack.
	virtual void NormalAttackUI() override;
	virtual void NormalAttack() override;
	
};
