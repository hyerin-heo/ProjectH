// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/PHBossDragon.h"

APHBossDragon::APHBossDragon()
{
}

void APHBossDragon::BeginPlay()
{
	Super::BeginPlay();
}

void APHBossDragon::AttackAction()
{
	Super::AttackAction();
}

void APHBossDragon::AttackHitCheck()
{
	Super::AttackHitCheck();
}

void APHBossDragon::Pattern1()
{
}

void APHBossDragon::Pattern2()
{
}

void APHBossDragon::Pattern3()
{
}

void APHBossDragon::Pattern4()
{
}

void APHBossDragon::Pattern1HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
}

void APHBossDragon::Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
}

void APHBossDragon::Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
}

void APHBossDragon::Pattern4HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
}

void APHBossDragon::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
	Super::PatternHitCheck(InPatternIndex, InStep);
}

void APHBossDragon::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
{
	Super::PhaseLevelChanged(OldPhase, NewPhase);
}
