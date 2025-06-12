// Fill out your copyright notice in the Description page of Project Settings.


#include "PHBossBiochemical.h"

#include "ProjectH.h"
#include "Game/PHGameMode.h"
#include "SkillObject/PHSwordAuraSkillObject.h"
#include "Subsystem/SkillObjectPoolSubsystem.h"

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

void APHBossBiochemical::Pattern5()
{
	// @PHTODO 패턴 대미지 위치 알려주는 UI및 이펙트 처리?
}

void APHBossBiochemical::Pattern1HitCheck(const FBossPatternInfo& PatternInfo)
{
	// @PHTODO 프로젝타일 날리기
	Test(PatternInfo.AttackDamage);
}

void APHBossBiochemical::Pattern2HitCheck(const FBossPatternInfo& PatternInfo)
{
	// @PHTODO 프로젝타일 날리기
	Test(PatternInfo.AttackDamage);
}

void APHBossBiochemical::Pattern3HitCheck(const FBossPatternInfo& PatternInfo)
{
	// @PHTODO 프로젝타일 날리기
	Test(PatternInfo.AttackDamage);
}

void APHBossBiochemical::Pattern4HitCheck(const FBossPatternInfo& PatternInfo)
{
	// @PHTODO 프로젝타일 날리기
	Test(PatternInfo.AttackDamage);
}

void APHBossBiochemical::Pattern5HitCheck(const FBossPatternInfo& PatternInfo)
{
	// @PHTODO 프로젝타일 날리기
	Test(PatternInfo.AttackDamage);
}

void APHBossBiochemical::Test(float Damage)
{
	// 월드 컨텍스트 객체를 사용하여 서브시스템 인스턴스 가져오기
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
		APHSwordAuraSkillObject* NewSkillObject = Cast<APHSwordAuraSkillObject>(PoolSubsystem->SpawnAndFireProjectile(
			SkillObjectsMap.FindChecked(SkillObjectType::SwordAura),
			GetActorLocation(),
			GetActorRotation(),
			this,
			this,
			Damage,
			1000.f,
			10.f
		));

		// if (NewSkillObject)
		// {
		// 	// 발사 방향 설정 (Subsystem에서 위치/회전은 이미 설정됨)
		// 	FVector ShotDirection = SpawnRotation.Vector();
		// 	NewSkillObject->FireInDirection(ShotDirection);
		// }
	}
}

void APHBossBiochemical::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
	Super::PatternHitCheck(InPatternIndex, InStep);
	int32 Pattern = InPatternIndex + 1;
	const FBossPatternInfo* PatternInfo = DataAsset->Patterns.Find(Pattern);
	switch (Pattern)
	{
	case  1:
		Pattern1HitCheck(*PatternInfo);
		break;
	case  2:
		Pattern2HitCheck(*PatternInfo);
		break;
	case  3:
		Pattern3HitCheck(*PatternInfo);
		break;
	case  4:
		Pattern4HitCheck(*PatternInfo);
		break;
	case  5:
		Pattern5HitCheck(*PatternInfo);
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
