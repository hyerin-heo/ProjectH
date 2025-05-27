// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/PHBossBiochemical.h"

#include "ProjectH.h"

APHBossBiochemical::APHBossBiochemical()
{
	CommonAttackPatternActions.Add(FCommonAttackPatternDelegateWrapper(FOnCommonAttackPattern::CreateUObject(this, &APHBossBiochemical::Attack1)));
	CommonAttackPatternActions.Add(FCommonAttackPatternDelegateWrapper(FOnCommonAttackPattern::CreateUObject(this, &APHBossBiochemical::Attack2)));
	CommonAttackPatternActions.Add(FCommonAttackPatternDelegateWrapper(FOnCommonAttackPattern::CreateUObject(this, &APHBossBiochemical::Attack3)));
	
	SpecialAttackPatternActions.Add(FSpecialAttackPatternDelegateWrapper(FOnSpecialAttackPattern::CreateUObject(this, &APHBossBiochemical::SpecialAttack1)));
	SpecialAttackPatternActions.Add(FSpecialAttackPatternDelegateWrapper(FOnSpecialAttackPattern::CreateUObject(this, &APHBossBiochemical::SpecialAttack2)));
	SpecialAttackPatternActions.Add(FSpecialAttackPatternDelegateWrapper(FOnSpecialAttackPattern::CreateUObject(this, &APHBossBiochemical::SpecialAttack3)));
}

void APHBossBiochemical::BeginPlay()
{
	Super::BeginPlay();
}

void APHBossBiochemical::Attack1()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Attack1"));
}

void APHBossBiochemical::Attack2()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Attack2"));
}

void APHBossBiochemical::Attack3()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Attack3"));
}

void APHBossBiochemical::SpecialAttack1()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::SpecialAttack1"));
}

void APHBossBiochemical::SpecialAttack2()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::SpecialAttack2"));
}

void APHBossBiochemical::SpecialAttack3()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::SpecialAttack3"));
}
