// Fill out your copyright notice in the Description page of Project Settings.


#include "PHBossCharacterBase.h"

// Sets default values
APHBossCharacterBase::APHBossCharacterBase()
{

}

// Called when the game starts or when spawned
void APHBossCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentPhaseLevel = 0;

	checkf(DataAsset, TEXT("DataAsset must be exist. \nCall Stack : %s"), ANSI_TO_TCHAR(__FUNCTION__));
	
	if (DataAsset)
	{
		MaxHP = DataAsset->MaxHp;
		HP = MaxHP;
		PhaseMap = DataAsset->PhaseMap;
		DetectionRadius = DataAsset->DetectionRadius;
		Speed = DataAsset->Speed;
		AttackRange = DataAsset->AttackRange;
		AttackSpeed = DataAsset->AttackSpeed;
		Armor = DataAsset->Armor;
		bIsUnableToAttack = false;
	}
}

void APHBossCharacterBase::CommonPattern()
{
	checkf(CommonAttackPatternActions.Num() > 0, TEXT("Common attack pattern action is empty!"));

	int index = FMath::RandRange(0, CommonAttackPatternActions.Num() - 1);

	CommonAttackPatternActions[index].ItemDelegate.Execute();
}

void APHBossCharacterBase::SpecialPattern()
{
	checkf(SpecialAttackPatternActions.Num() > 0, TEXT("Special attack pattern action is empty!"));

	int index = FMath::RandRange(0, SpecialAttackPatternActions.Num() - 1);

	SpecialAttackPatternActions[index].ItemDelegate.Execute();
}

void APHBossCharacterBase::SetPhase(uint8 level)
{
	CurrentPhaseLevel = level;
	if (const FBossPhaseInfo* FoundInfo = PhaseMap.Find(level))
	{
		TriggerType = FoundInfo->TriggerType;
		TriggerValue = FoundInfo->TriggerValue;
		// @PHTODO 맞았을 시 트리거 타입과 밸류 측정해서 다음페이즈 실행하기
		if (TriggerType == EBossPhaseTriggerType::Timer)
		{
			GetWorldTimerManager().SetTimer(PhaseTimerHandle, [&]()
			{
				SpecialPattern();
			}, TriggerValue, false);
		}
	}
}

