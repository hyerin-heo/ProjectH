// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHWarriorCharacter.h"

#include "ProjectH.h"

APHWarriorCharacter::APHWarriorCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/AM_TwoHandMontage.AM_TwoHandMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}
}

void APHWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHWarriorCharacter::NormalAttackUI()
{
	PH_LOG(LogPHCharacter, Log, TEXT("SHOW NormalAttackUI"));
	Super::NormalAttackUI();
}

void APHWarriorCharacter::NormalAttack()
{
	PlaySectionName = "NormalAttack";
	PH_LOG(LogPHCharacter, Log, TEXT("SHOW NormalAttack!!!!!!"));
	Super::NormalAttack();

	PlayAnimMontage(ActionMontage, 1.0f, PlaySectionName);
	
	
}
