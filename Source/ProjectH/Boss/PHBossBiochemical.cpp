// Fill out your copyright notice in the Description page of Project Settings.


#include "PHBossBiochemical.h"

#include "SkillObject/PHNiagaraSkillObject.h"

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
}

void APHBossBiochemical::Pattern1()
{
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리? 
}

void APHBossBiochemical::Pattern2()
{
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern3()
{
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern4()
{
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리? 
}

#pragma region Pattern 1

void APHBossBiochemical::Pattern1HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura);
	const FVector BaseSpawnLocation = GetActorLocation();
	const float BaseSpeed = 1000.f;
	const float BaseLifetime = 3.f;
	const int32 NumProjectiles = 8 * (Step + 1);

	const float AngleStep = 360.f / NumProjectiles;

	for (int32 i = 0; i < NumProjectiles; ++i)
	{
		FRotator SpawnRotation = GetActorRotation() + FRotator(0, AngleStep * i, 0);
		ASkillObjectBase* SkillObject = SpawnSkillObject(
			SkillClass,
			BaseSpawnLocation,
			SpawnRotation
		);

		LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, PatternInfo.AttackDamage, true);
	}
}

#pragma endregion


#pragma region Pattern 2

void APHBossBiochemical::Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(SkillObjectType::DarkGreenSwordAura);
	const FVector BaseSpawnLocation = GetActorLocation();
	const float BaseSpeed = 1500.f;
	const float BaseLifetime = 3.f;
	TArray<float> Angles = {0.f, -10.f, 10.f, -20.f, 20.f, -30.f, 30.f};

	for (float Angle : Angles)
	{
		FRotator SpawnRotation = GetActorRotation() + FRotator(0, Angle, 0);
		ASkillObjectBase* SkillObject = SpawnSkillObject(
			SkillClass,
			BaseSpawnLocation,
			SpawnRotation
		);
		LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, PatternInfo.AttackDamage, false);
	}
}

#pragma endregion


#pragma region Pattern 3

void APHBossBiochemical::Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(SkillObjectType::StoneRush);
	const float BaseLifetime = 4.5f;

	// 5방향으로 AttackRange만큼 떨어진 지점에 스폰
	const int32 NumProjectiles = 6;
	const float AngleStep = 360.f / NumProjectiles;

	for (int32 i = 0; i < NumProjectiles; ++i)
	{
		FRotator CurrentRotation = GetActorRotation() + FRotator(0.f, AngleStep * i, 0.f);
		FVector SpawnLocation = GetActorLocation() + (CurrentRotation.Vector() * PatternInfo.AttackRange) - FVector(
			0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

		ASkillObjectBase* SkillObject = SpawnSkillObject(
			SkillClass,
			SpawnLocation,
			CurrentRotation
		);
		LaunchSkillObject(SkillObject, BaseLifetime, PatternInfo.AttackDamage);
	}
}

#pragma endregion


#pragma region Pattern 4

void APHBossBiochemical::Pattern4HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	if (Step == 2) //Ground Attack
	{
		const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(SkillObjectType::DashMagma);
		const float BaseLifetime = 4.5f;

		TArray<float> Angles = {0.f, 15.f, -15.f, 30.f, -30.f};

		for (float Angle : Angles)
		{
			FRotator CurrentRotation = GetActorRotation() + FRotator(0.f, Angle, 0.f);
			FVector SpawnLocation = GetActorLocation() + (CurrentRotation.Vector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

			ASkillObjectBase* SkillObject = SpawnSkillObject(
				SkillClass,
				SpawnLocation,
				CurrentRotation
			);
			LaunchSkillObject(SkillObject, BaseLifetime, PatternInfo.AttackDamage);
		}
	}
	else
	{
		const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.
			FindChecked(SkillObjectType::DarkRedSwordAura);
		const float BaseSpeed = 2000.f;
		const float BaseLifetime = 3.f;
		const FVector BaseSpawnLocation = GetActorLocation();

		TArray<float> Angles = {5.f, -5.f, 15.f, -15.f, 25.f, -25.f, 35.f, -35.f};

		for (float Angle : Angles)
		{
			FRotator SpawnRotation = GetActorRotation() + FRotator(0.f, Angle, 0.f);
			ASkillObjectBase* SkillObject = SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation,
				SpawnRotation
			);
			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, PatternInfo.AttackDamage, false);
		}
	}
}

#pragma endregion

void APHBossBiochemical::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
	Super::PatternHitCheck(InPatternIndex, InStep);
	int32 Pattern = InPatternIndex + 1;
	const FBossPatternInfo* PatternInfo = DataAsset->Patterns.Find(Pattern);
	switch (Pattern)
	{
	case 1:
		Pattern1HitCheck(*PatternInfo, InStep);
		break;
	case 2:
		Pattern2HitCheck(*PatternInfo, InStep);
		break;
	case 3:
		Pattern3HitCheck(*PatternInfo, InStep);
		break;
	case 4:
		Pattern4HitCheck(*PatternInfo, InStep);
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
	case 1:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern1),
			                              *PatternInfo));
		break;
	case 2:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern2),
			                              *PatternInfo));
		break;
	case 3:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern3),
			                              *PatternInfo));
		break;
	case 4:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossBiochemical::Pattern4),
			                              *PatternInfo));
		break;
	}
}
