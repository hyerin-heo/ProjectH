// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHHealerCharacter.h"

#include "ProjectH.h"
#include "Common/GlobalEnum.h"
#include "Net/UnrealNetwork.h"
#include "Component/PHCharacterStatComponent.h"

APHHealerCharacter::APHHealerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/Healer/AM_HealerMontage.AM_HealerMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}
	
	// Weapon Component
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
}

void APHHealerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHHealerCharacter, InvincibilityRemainingTime);
}

void APHHealerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHHealerCharacter::OnPossessed()
{
	Super::OnPossessed();
}

void APHHealerCharacter::NormalAttackUI()
{
	Super::NormalAttackUI();
}

void APHHealerCharacter::NormalAttack()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack));
		return;
	}
	
	Super::NormalAttack();

	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "NormalAttack");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);	
	}

	ServerRPCNormalAttack();
}

void APHHealerCharacter::SpawnNormalAttackObject()
{
	if (!HasAuthority())
		return;
	
	const FVector BaseSpawnLocation = GetActorLocation();
	const float BaseSpeed = 1000.f;
	const float BaseLifetime = 3.f;

	FRotator SpawnRotation = GetActorRotation();
	ASkillObjectBase* SkillObject = SpawnSkillObject(
		NormalAttackObject,
		BaseSpawnLocation,
		SpawnRotation
		
	);

	LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, StatDataComponent->GetDamage(EAttackType::DefaultAttack), true);
}
