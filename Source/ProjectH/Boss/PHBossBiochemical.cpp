// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/PHBossBiochemical.h"

#include "ProjectH.h"

APHBossBiochemical::APHBossBiochemical()
{
	
}

void APHBossBiochemical::BeginPlay()
{
	Super::BeginPlay();
	
	// checkf(DataAsset->PhaseMap.Num() > DataAsset->Patterns.Num(), TEXT("Check Boss Pattern Info!"));
	
}

void APHBossBiochemical::AttackAction()
{
	Super::AttackAction();
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Attack"));
}

void APHBossBiochemical::Pattern1()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern1"));
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리? 
}

void APHBossBiochemical::Pattern2()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern2"));
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern3()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern3"));
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern4()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern4"));
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리? 
}

void APHBossBiochemical::Pattern5()
{
	PH_LOG(LogPHBoss, Log, TEXT("Called APHBossBiochemical::Pattern5"));
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern1HitCheck()
{
	// @PHTODO 프로젝타일 날리기
}

void APHBossBiochemical::Pattern2HitCheck()
{
	// @PHTODO 프로젝타일 날리기
}

void APHBossBiochemical::Pattern3HitCheck()
{
	// @PHTODO 프로젝타일 날리기
}

void APHBossBiochemical::Pattern4HitCheck()
{
	// @PHTODO 프로젝타일 날리기
}

void APHBossBiochemical::Pattern5HitCheck()
{
	// @PHTODO 프로젝타일 날리기
}

void APHBossBiochemical::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
	Super::PatternHitCheck(InPatternIndex, InStep);

	switch (InPatternIndex)
	{
	case  1:
		Pattern1HitCheck();
		break;
	case  2:
		Pattern2HitCheck();
		break;
	case  3:
		Pattern3HitCheck();
		break;
	case  4:
		Pattern4HitCheck();
		break;
	case  5:
		Pattern5HitCheck();
		break;
	}
}

void APHBossBiochemical::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
{
	Super::PhaseLevelChanged(OldPhase, NewPhase);
	if (!DataAsset)
	{
		return;
	}

	const FBossPatternInfo* PatternInfo = DataAsset->Patterns.Find(CurrentPhaseLevel.GetValue());

	switch (CurrentPhaseLevel.GetValue())
	{
	case  1:
		AttackPatternActions.Add(FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern1), *PatternInfo));
		break;
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
