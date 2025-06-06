// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/PHBossBiochemical.h"

#include "ProjectH.h"

APHBossBiochemical::APHBossBiochemical()
{
	
}

void APHBossBiochemical::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(AttackPatternActions.Num() > DataAsset->Patterns.Num(), TEXT("Check Boss Pattern Info!"));
	
	const FBossPatternInfo* PatternInfo = DataAsset->Patterns.Find(1);
	AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern1), *PatternInfo));
}

void APHBossBiochemical::AttackAction()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Attack"));
}

void APHBossBiochemical::Pattern1()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern1"));
}

void APHBossBiochemical::Pattern2()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern2"));
}

void APHBossBiochemical::Pattern3()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern3"));
}

void APHBossBiochemical::Pattern4()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern4"));
}

void APHBossBiochemical::Pattern5()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern5"));
}

void APHBossBiochemical::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
{
	Super::PhaseLevelChanged(OldPhase, NewPhase);

	const FBossPatternInfo* PatternInfo = DataAsset->Patterns.Find(CurrentPhaseLevel.GetValue());

	switch (CurrentPhaseLevel.GetValue())
	{
	case  2:
		AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern2), *PatternInfo));
		break;
	case  3:
		AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern3), *PatternInfo));
		break;
	case  4:
		AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern4), *PatternInfo));
		break;
	case  5:
		AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern5), *PatternInfo));
		break;
	}
}
