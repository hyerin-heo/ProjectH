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
		
	}
}

void APHBossCharacterBase::CommonPattern()
{
}

void APHBossCharacterBase::SpecialPattern()
{
}

void APHBossCharacterBase::SetPhase(uint8 level)
{
	CurrentPhaseLevel = level;
	if (const FBossPhaseInfo* FoundInfo = PhaseMap.Find(level))
	{
		TriggerType = FoundInfo->TriggerType;
		TriggerValue = FoundInfo->TriggerValue;
	
		
	}
}

