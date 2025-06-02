// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHWarriorCharacter.h"

#include "ProjectH.h"
#include "GameFramework/CharacterMovementComponent.h"

APHWarriorCharacter::APHWarriorCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/AM_TwoHandMontage.AM_TwoHandMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}

	// Weapon Component
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
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
	Super::NormalAttack();
	
	if (!HasAuthority())
	{
	
		PlayAnimMontage(ActionMontage, 1.0f, PlaySectionName);
		SetMontageEndDelegate();	
	}

	ServerRPCNormalAttack();
}
