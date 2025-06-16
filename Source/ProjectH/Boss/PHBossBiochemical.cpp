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

#pragma region Pattern 1

void APHBossBiochemical::Pattern1HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));
		switch (Step)
		{
			case 0:
				{
					APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation(),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f, 0),
						this,
						this
						));
					
					APHNiagaraSkillObject* NewSkillObject4 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f+45.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject5 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f+45.f+45.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject6 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f+45.f+45.f+45.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject7 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f+45.f+45.f+45.f+45.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject8 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 45.f+45.f+45.f+45.f+45.f+45.f+45.f, 0),
						this,
						this
					));

					if (NewSkillObject)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject->Init(500.f, 3.f, true);
						NewSkillObject->Launch(GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject2)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject2->Init(500.f, 3.f, true);
						NewSkillObject2->Launch(NewSkillObject2->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject3)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject3->Init(500.f, 3.f, true);
						NewSkillObject3->Launch(NewSkillObject3->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject4)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject4->Init(500.f, 3.f, true);
						NewSkillObject4->Launch(NewSkillObject4->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject5)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject5->Init(500.f, 3.f, true);
						NewSkillObject5->Launch(NewSkillObject5->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject6)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject6->Init(500.f, 3.f, true);
						NewSkillObject6->Launch(NewSkillObject6->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject7)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject7->Init(500.f, 3.f, true);
						NewSkillObject7->Launch(NewSkillObject7->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject8)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject8->Init(500.f, 3.f, true);
						NewSkillObject8->Launch(NewSkillObject8->GetActorForwardVector(), PatternInfo.AttackDamage);
					}
				}
				break;
			case 1:
				{
					APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation(),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 25.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, -25.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject4 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, 50.f, 0),
						this,
						this
						));

					APHNiagaraSkillObject* NewSkillObject5 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
						SkillObjectsMap.FindChecked(SkillObjectType::GreenSwordAura),
						GetActorLocation(),
						GetActorRotation() + FRotator(0, -50.f, 0),
						this,
						this
					));

					if (NewSkillObject)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject->Init(500.f, 3.f, false);
						NewSkillObject->Launch(GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject2)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject2->Init(500.f, 3.f, false);
						NewSkillObject2->Launch(NewSkillObject2->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject3)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject3->Init(500.f, 3.f, false);
						NewSkillObject3->Launch(NewSkillObject3->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject4)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject4->Init(500.f, 3.f, false);
						NewSkillObject4->Launch(NewSkillObject4->GetActorForwardVector(), PatternInfo.AttackDamage);
					}

					if (NewSkillObject5)
					{
						// @PHTODO 투사체 속도 세팅 필요
						NewSkillObject5->Init(500.f, 3.f, false);
						NewSkillObject5->Launch(NewSkillObject5->GetActorForwardVector(), PatternInfo.AttackDamage);
					}
				}
				break;
			default:
				break;
		}
	}
}

#pragma endregion



#pragma region Pattern 2

void APHBossBiochemical::Pattern2HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
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

		APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::DarkGreenSwordAura),
			GetActorLocation(),
			GetActorRotation()+ FRotator(0.0f,-30.0f,0.0f),
			this,
			this
		));

		APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::DarkGreenSwordAura),
			GetActorLocation(),
			GetActorRotation() + FRotator(0.0f,30.0f,0.0f),
			this,
			this
		));

		if (NewSkillObject)
		{
			// @PHTODO 투사체 속도 세팅 필요
			NewSkillObject->Init(500.f, 3.f, false);
			NewSkillObject->Launch(NewSkillObject->GetActorForwardVector(), PatternInfo.AttackDamage);
		}
		if (NewSkillObject2)
		{
			// @PHTODO 투사체 속도 세팅 필요
			NewSkillObject2->Init(500.f, 3.f, false);
			NewSkillObject2->Launch(NewSkillObject2->GetActorForwardVector(), PatternInfo.AttackDamage);
		}
		if (NewSkillObject3)
		{
			// @PHTODO 투사체 속도 세팅 필요
			NewSkillObject3->Init(500.f, 3.f, false);
			NewSkillObject3->Launch(NewSkillObject3->GetActorForwardVector(), PatternInfo.AttackDamage);
		}
	}
}

#pragma endregion



#pragma region Pattern 3

void APHBossBiochemical::Pattern3HitCheck(const FBossPatternInfo& PatternInfo, const uint8& Step)
{
	USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	if (PoolSubsystem)
	{
		// const FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT(""));

		APHNiagaraSkillObject* NewSkillObject = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + (GetActorForwardVector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation(),
			this,
			this
		));
		APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + ((GetActorRotation() + FRotator(0.f, 72.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation()+ FRotator(0.f, 72.f, 0.f),
			this,
			this
			));
		APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + ((GetActorRotation() + FRotator(0.f, 72.f+72.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation()+ FRotator(0.f, 72.f+72.f, 0.f),
			this,
			this
			));
		APHNiagaraSkillObject* NewSkillObject4 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + ((GetActorRotation() + FRotator(0.f, 72.f+72.f+72.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation()+ FRotator(0.f, 72.f+72.f+72.f, 0.f),
			this,
			this
			));
		APHNiagaraSkillObject* NewSkillObject5 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
			SkillObjectsMap.FindChecked(SkillObjectType::StoneRush),
			GetActorLocation() + ((GetActorRotation() + FRotator(0.f, 72.f+72.f+72.f+72.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
				0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
			GetActorRotation() + FRotator(0.f, 72.f+72.f+72.f+72.f, 0.f),
			this,
			this
		));

		if (NewSkillObject)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject->Launch(PatternInfo.AttackDamage, 4.5f);
		}
		if (NewSkillObject2)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject2->Launch(PatternInfo.AttackDamage, 4.5f);
		}
		if (NewSkillObject3)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject3->Launch(PatternInfo.AttackDamage, 4.5f);
		}
		if (NewSkillObject4)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject4->Launch(PatternInfo.AttackDamage, 4.5f);
		}
		if (NewSkillObject5)
		{
			// @PHTODO 투사체 생명 주기 세팅 필요
			NewSkillObject5->Launch(PatternInfo.AttackDamage, 4.5f);
		}
	}
}

#pragma endregion



#pragma region Pattern 4

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
				GetActorLocation() + (GetActorForwardVector() * PatternInfo.AttackRange) - FVector(
					0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
				GetActorRotation(),
				this,
				this
				));

			APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DashMagma),
				GetActorLocation() + ((GetActorRotation() + FRotator(0.f, 25.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
					0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
				GetActorRotation() + FRotator(0.f, 25.f, 0.f),
				this,
				this
				));

			APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DashMagma),
				GetActorLocation() + ((GetActorRotation() + FRotator(0.f, -25.f, 0.f)).Vector() * PatternInfo.AttackRange) - FVector(
					0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
				GetActorRotation() + FRotator(0.f, -25.f, 0.f),
				this,
				this
			));

			if (NewSkillObject)
			{
				// @PHTODO 투사체 생명 주기 세팅 필요
				NewSkillObject->Launch(PatternInfo.AttackDamage, 4.5f);
			}

			if (NewSkillObject2)
			{
				// @PHTODO 투사체 생명 주기 세팅 필요
				NewSkillObject2->Launch(PatternInfo.AttackDamage, 4.5f);
			}

			if (NewSkillObject3)
			{
				// @PHTODO 투사체 생명 주기 세팅 필요
				NewSkillObject3->Launch(PatternInfo.AttackDamage, 4.5f);
			}
		}
	}
	else
	{
		USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
		if (PoolSubsystem)
		{
			APHNiagaraSkillObject* NewSkillObject1 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,5.f,0.f),
				this,
				this
				));
			APHNiagaraSkillObject* NewSkillObject2 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,-5.f,0.f),
				this,
				this
				));
			APHNiagaraSkillObject* NewSkillObject3 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,15.f,0.f),
				this,
				this
				));
			APHNiagaraSkillObject* NewSkillObject4 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,-15.f,0.f),
				this,
				this
				));
			APHNiagaraSkillObject* NewSkillObject5 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,25.f,0.f),
				this,
				this
				));
			APHNiagaraSkillObject* NewSkillObject6 = Cast<APHNiagaraSkillObject>(PoolSubsystem->SpawnSkillObject(
				SkillObjectsMap.FindChecked(SkillObjectType::DarkRedSwordAura),
				GetActorLocation(),
				GetActorRotation() + FRotator(0.f,-25.f,0.f),
				this,
				this
			));

			if (NewSkillObject1)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject1->Init(500.f, 3.f, false);
				NewSkillObject1->Launch(NewSkillObject1->GetActorForwardVector(), PatternInfo.AttackDamage);
			}

			if (NewSkillObject2)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject2->Init(500.f, 3.f, false);
				NewSkillObject2->Launch(NewSkillObject2->GetActorForwardVector(), PatternInfo.AttackDamage);
			}

			if (NewSkillObject3)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject3->Init(500.f, 3.f, false);
				NewSkillObject3->Launch(NewSkillObject3->GetActorForwardVector(), PatternInfo.AttackDamage);
			}

			if (NewSkillObject4)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject4->Init(500.f, 3.f, false);
				NewSkillObject4->Launch(NewSkillObject4->GetActorForwardVector(), PatternInfo.AttackDamage);
			}

			if (NewSkillObject5)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject5->Init(500.f, 3.f, false);
				NewSkillObject5->Launch(NewSkillObject5->GetActorForwardVector(), PatternInfo.AttackDamage);
			}

			if (NewSkillObject6)
			{
				// @PHTODO 투사체 속도 세팅 필요
				NewSkillObject6->Init(500.f, 3.f, false);
				NewSkillObject6->Launch(NewSkillObject6->GetActorForwardVector(), PatternInfo.AttackDamage);
			}
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
