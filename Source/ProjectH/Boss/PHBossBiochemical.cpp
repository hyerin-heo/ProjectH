// Fill out your copyright notice in the Description page of Project Settings.


#include "PHBossBiochemical.h"

#include "SkillObject/PHNiagaraSkillObject.h"
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

void APHBossBiochemical::Pattern1HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
		APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
			GetActorLocation(),
			GetActorRotation(),
			this,
			this
		));

		if (NewSkillObject)
		{
			// @PHTODO 투사체 속도 세팅 필요
			NewSkillObject->Init(500.f, 3.f, false);
			NewSkillObject->Launch(GetActorForwardVector(),PatternInfo.AttackDamage);
		}
	}
}

void APHBossBiochemical::Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	// @PHTODO 다른 프로젝타일로 할지?
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
		APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::DarkGreenSwordAura),
			GetActorLocation(),
			GetActorRotation(),
			this,
			this
		));

		if (NewSkillObject)
		{
			// @PHTODO 투사체 속도 세팅 필요
			NewSkillObject->Init(500.f, 3.f, false);
			NewSkillObject->Launch(GetActorForwardVector(),PatternInfo.AttackDamage);
		}
	}
}

void APHBossBiochemical::Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
		APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + (GetActorForwardVector() * 500.f) - FVector(0.f,0.f,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation(),
			this,
			this
		));

		if (NewSkillObject)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject->Launch(PatternInfo.AttackDamage, 4.5f);
		}
	}
}

void APHBossBiochemical::Pattern4HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	if (Step == 2)
	{
		USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
		if (PoolSubsystem)
		{
			// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
			APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DashMagma),
				GetActorLocation() + (GetActorForwardVector() * PatternInfo.AttackRange) - FVector(0.f,0.f,GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
				GetActorRotation(),
				this,
				this
			));

			if (NewSkillObject)
			{
				// @PHTODO 투사체 생명 주기 세팅 필요
				NewSkillObject->Launch(PatternInfo.AttackDamage, 4.5f);
			}
		}
	}else
	{
		USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
		if (PoolSubsystem)
		{
			// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		
			APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation(),
				this,
				this
			));

			if (NewSkillObject)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject->Init(500.f, 3.f, false);
				NewSkillObject->Launch(GetActorForwardVector(),PatternInfo.AttackDamage);
			}
		}
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
		Pattern1HitCheck(*PatternInfo, InStep);
		break;
	case  2:
		Pattern2HitCheck(*PatternInfo, InStep);
		break;
	case  3:
		Pattern3HitCheck(*PatternInfo, InStep);
		break;
	case  4:
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
	}
}
