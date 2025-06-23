// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHMageCharacter.h"

#include "ProjectH.h"
#include "Common/GlobalEnum.h"

APHMageCharacter::APHMageCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	Weapon->SetIsReplicated(true);
}

void APHMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHMageCharacter::SkillEffect(EAttackType InAttackType, uint8 InStep)
{
	switch (InAttackType)
	{
	case EAttackType::DefaultAttack:
		// @PHTODO 파이어볼 프로젝타일 쏘기
		break;
	case EAttackType::Skill1:
		// @PHTODO 레이저 발사
		break;
	case EAttackType::Skill2:
		// @PHTODO 프로젝타일 여러방향으로 쏘기
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
}

void APHMageCharacter::ServerRPCSkill3_Implementation()
{
	Super::ServerRPCSkill3_Implementation();
}

void APHMageCharacter::ServerRPCSkill4_Implementation()
{
	Super::ServerRPCSkill4_Implementation();
}

void APHMageCharacter::NormalAttack()
{
	Super::NormalAttack();
}

void APHMageCharacter::Skill1()
{
	Super::Skill1();
}

void APHMageCharacter::Skill2()
{
	Super::Skill2();
}

void APHMageCharacter::Skill3()
{
	Super::Skill3();
}

void APHMageCharacter::Skill4()
{
	Super::Skill4();
}
