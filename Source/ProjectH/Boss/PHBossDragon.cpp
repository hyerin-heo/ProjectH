// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/PHBossDragon.h"

#include "NavigationSystem.h"
#include "Common/SkillObject/PHProjectileSkillObject.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/PHSoundManager.h"

APHBossDragon::APHBossDragon()
{
}

void APHBossDragon::BeginPlay()
{
	Super::BeginPlay();
}


void APHBossDragon::AttackAction()
{
	int32 Index = -1;
	Index = FMath::RandRange(1, DRAGONATTACKTYPECOUNT);
	FName MontageName = FName(*FString::Printf(TEXT("%s%d"),*DefaultActionName.ToString(), Index));
    
	PlayAnimMontage(ActionMontage, AttackSpeed, MontageName);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APHBossCharacterBase::AttackActionEnd);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);

	// client에 attack 전송
	AttackActionRPC();
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
	
	GetWorld()->GetTimerManager().SetTimer(Pattern1TimerHandle, FTimerDelegate::CreateLambda([&]
	{
		if (!bIsPattern)
		{
			GetWorld()->GetTimerManager().ClearTimer(Pattern1TimerHandle);
			return;
		}

		const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(EDragonSkillObjectType::DragonSkillObject1);
		const FVector BoneLocation = GetMesh()->GetBoneLocation(TEXT("Extra_Tongue3_Joint"), EBoneSpaces::WorldSpace);
		const FVector BaseSpawnLocation = FVector(BoneLocation.X, BoneLocation.Y, GetActorLocation().Z - 70.0f);
		const float BaseSpeed = 1000.f;
		const float BaseLifetime = 3.f;

		const int32 NumProjectiles = 3;
		const float AngleStep = 120.f / NumProjectiles;

		for (int32 i = 0; i < NumProjectiles; ++i)
		{
			FRotator SpawnRotation = GetActorRotation() + FRotator(0, AngleStep * i, 0);
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
			SkillClass,
			BaseSpawnLocation,
			SpawnRotation
			));

			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, PatternInfo.AttackDamage, true);	
		}

		if (UWorld* World = GetWorld())
		{
			if (UGameInstance* GI = World->GetGameInstance())
			{
				if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
				{
					SoundManager->PlaySFX(ESoundCategory::SFX, TEXT("DragonFire"), 0.7f);
				}
			}
		}
		
	}), 0.25f, true);	
}

void APHBossDragon::Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	GetWorld()->GetTimerManager().SetTimer(Pattern2TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (!bIsPattern)
		{
			GetWorld()->GetTimerManager().ClearTimer(Pattern2TimerHandle);
			InnerStep = 0;
			return;
		}

		const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(EDragonSkillObjectType::DragonSkillObject1);
		const FVector BaseSpawnLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - 70.0f);
		const float BaseSpeed = 1000.f;
		const float BaseLifetime = 3.f;

		const int32 NumProjectiles = 12;
		const float AngleStep = 360.0f / NumProjectiles;

		const float AdditionalAngleOffset = (InnerStep % 2 == 0) ? 0.0f : AngleStep / 2.0f;

		for (int32 i = 0; i < NumProjectiles; ++i)
		{
			const float Angle = i * AngleStep + AdditionalAngleOffset;
			FRotator SpawnRotation = GetActorRotation() + FRotator(0, Angle, 0);

			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(
				SpawnSkillObject(SkillClass, BaseSpawnLocation, SpawnRotation)
			);

			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, PatternInfo.AttackDamage, true);
		}

		++InnerStep;

	}), 0.5f, true);
}

void APHBossDragon::Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(EDragonSkillObjectType::DragonSkillObject2);
	const float BaseLifetime = 1.0f;

	// 5방향으로 AttackRange만큼 떨어진 지점에 스폰
	const int32 NumProjectiles = 40;
	const FVector Origin = GetActorLocation();
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;
	
	for (int32 i = 0; i < NumProjectiles; ++i)
	{
		FNavLocation OutLocation;

		bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, 2000.f, OutLocation);

		if (bFound)
		{
			FRotator CurrentRotation = GetActorRotation();
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(
				SpawnSkillObject(SkillClass, OutLocation.Location, FRotator::ZeroRotator));
			
			LaunchSkillObject(SkillObject, BaseLifetime, PatternInfo.AttackDamage, 0.8f);
		}
		
	}
	
}

void APHBossDragon::Pattern4HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	GetWorld()->GetTimerManager().SetTimer(Pattern4TimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		if (!bIsPattern)
		{
			GetWorld()->GetTimerManager().ClearTimer(Pattern4TimerHandle);
			return;
		}
		
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (!NavSys) return;

		const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.FindChecked(EDragonSkillObjectType::DragonSkillObject3);
		const FVector Origin = GetActorLocation();
		FNavLocation OutLocation;
		const float BaseLifetime = 3.5f;
		
		bool bFound = NavSys->GetRandomReachablePointInRadius(Origin, 2000.f, OutLocation);

		if (bFound)
		{
			FRotator CurrentRotation = GetActorRotation();
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(
				SpawnSkillObject(SkillClass, OutLocation.Location, FRotator::ZeroRotator));
			
			LaunchSkillObject(SkillObject, BaseLifetime, PatternInfo.AttackDamage, 1.25f);
		}

	}), 0.2f, true);
}

void APHBossDragon::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
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

void APHBossDragon::PlayPattern3LoopAnim(UAnimMontage* AnimMontage, bool bArg)
{
	
}

void APHBossDragon::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
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
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossDragon::Pattern1), *PatternInfo));
		break;
	case 2:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossDragon::Pattern2), *PatternInfo));
		break;
	case 3:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossDragon::Pattern3), *PatternInfo));
		break;
	case 4:
		AttackPatternActions.Add(
			FAttackPatternDelegateWrapper(FOnAttackPattern::CreateUObject(this, &APHBossDragon::Pattern4), *PatternInfo));
		break;
	}
}
