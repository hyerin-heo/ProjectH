// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHMageCharacter.h"

#include "ProjectH.h"
#include "Common/GlobalEnum.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "Component/PHCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APHMageCharacter::APHMageCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Weapon->SetIsReplicated(true);
}

void APHMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHMageCharacter::SetSkill(EAttackType InAttackType, uint8 InStep)
{
	float Damage = StatDataComponent->GetDamage(InAttackType);
	const float BaseLifetime = 2.f;
	const float BaseSpeed = 1500.0f;
	const FVector BaseSpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = GetActorRotation();
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.
	FindChecked(InAttackType);
	switch (InAttackType)
	{
	case EAttackType::DefaultAttack:
		{
			ASkillObjectBase* SkillObject = SpawnSkillObject(
					SkillClass,
					BaseSpawnLocation,
					SpawnRotation
				);
			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, Damage, true);
		}
		break;
	case EAttackType::Skill1:
		// @PHTODO 레이저 발사
		break;
	case EAttackType::Skill2:
		{
			ASkillObjectBase* SkillObjectBase = SkillClass->GetDefaultObject<ASkillObjectBase>();
			float HalfHeight = SkillObjectBase->CollisionComponent->GetScaledCapsuleHalfHeight();
			ASkillObjectBase* SkillObject = SpawnSkillObject(
					SkillClass,
					BaseSpawnLocation + FVector(0.0f, 0.0f, HalfHeight),
					SpawnRotation
				);
			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, Damage, false);
		}
		break;
	case EAttackType::Skill3:
		// @PHTODO 텔레포트
		break;
	case EAttackType::Skill4:
		// @PHTODO 아마겟돈 프로젝타일 쏘기
		break;
	default:
		PH_LOG(LogPHCharacter, Warning, TEXT("Invalid parameter! %s"), *ToString(InAttackType));
		break;
	}
}

void APHMageCharacter::ServerRPCSkill1_Implementation()
{
	Super::ServerRPCSkill1_Implementation();
}

void APHMageCharacter::ServerRPCSkill2_Implementation()
{
	Super::ServerRPCSkill2_Implementation();
	//override안해도 될듯
}

void APHMageCharacter::ServerRPCSkill3_Implementation()
{
	Super::ServerRPCSkill3_Implementation();
	StatDataComponent->StartSkillCooldown(EAttackType::Skill3);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill3");
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetTeleport();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3", 1.0f);
}

void APHMageCharacter::ServerRPCSkill4_Implementation()
{
	Super::ServerRPCSkill4_Implementation();
	//override안해도 될듯
}

void APHMageCharacter::NormalAttack()
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
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);	
	}

	ServerRPCNormalAttack();
}

void APHMageCharacter::Skill1()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill1))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill1));
		return;
	}
	Super::Skill1();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			// SetActionEnd();
			//loop실행하고 움직이지 못하도록 세팅. 움직임이 들어오면 그냥 끝내버리기.
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill1();
}

void APHMageCharacter::Skill2()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill2))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill2();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill2();
}

void APHMageCharacter::Skill3()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill3))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill3();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill3");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetTeleport();
			// SetActionEnd();
			//텔레포트하고 Skill3End실행
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill3();
}

void APHMageCharacter::Skill4()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill4))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill4();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill4");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill4();
}

void APHMageCharacter::SetTeleport()
{
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill3End");
		SendClientRPCPlayAnimation("Skill3End", 1.0f);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}
}
