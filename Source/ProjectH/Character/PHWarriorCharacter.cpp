// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHWarriorCharacter.h"

#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "ProjectH.h"
#include "Component/PHCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/PHWarriorSkill3Object.h"
#include "Kismet/GameplayStatics.h"

APHWarriorCharacter::APHWarriorCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/AM_TwoHandMontage.AM_TwoHandMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<APHWarriorSkill3Object> ClassSkill3ObjectRef(TEXT("/Game/ProjectH/Blueprints/Character/SkillObject/MyPHWarriorSkill3Object.MyPHWarriorSkill3Object_C"));

	if (ClassSkill3ObjectRef.Class)
	{
		SkillObjClass = ClassSkill3ObjectRef.Class;
	}

	Skill1Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashEffect"));
	Skill1Effect->SetupAttachment(RootComponent);
	Skill1Effect->SetAutoActivate(false);
	Skill2Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TornadoEffect"));
	Skill2Effect->SetupAttachment(RootComponent);
	Skill2Effect->SetAutoActivate(false);
	Skill4Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffect"));
	Skill4Effect->SetupAttachment(RootComponent);
	Skill4Effect->SetAutoActivate(false);
	
	// Weapon Component
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
}

void APHWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bTornadoSkill)
	{
		if (TornadoTurningTime > 0.0f)
		{
			TornadoTurningTime -= DeltaTime;
		}
		else
		{
			bTornadoSkill = false;
			TornadoTurningTime = MAX_TURNINGTIME;
			EndLoopTornadoSkill();	
		}
	}

	if (StatDataComponent->GetCooldownReduction() && HasAuthority())
	{
		if (BerserkSkillRemaining > 0.0f)
		{
			BerserkSkillRemaining -= DeltaTime;
		}
		else
		{
			EndBerserkSkill();
		}
	}
	
}

void APHWarriorCharacter::NormalAttackUI()
{
	Super::NormalAttackUI();
}

void APHWarriorCharacter::NormalAttack()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack));
		return;
	}
	
	Super::NormalAttack();
	
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "NormalAttack");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);	
	}

	ServerRPCNormalAttack();
}

void APHWarriorCharacter::Skill1UI()
{
	//PH_LOG(LogPHCharacter, Log, TEXT("SHOW Skill1UI"));
	Super::Skill1UI();
}

void APHWarriorCharacter::Skill1()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill1))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill1));
		return;
	}
	
	Super::Skill1();

	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill1();
}

void APHWarriorCharacter::Skill2UI()
{
	Super::Skill2UI();
}

void APHWarriorCharacter::Skill2()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill2))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	
	Super::Skill2();

	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.5f, "Skill2_Start");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			StartLoopTornadoSkill();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill2();
}

void APHWarriorCharacter::Skill3()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill3))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill3));
		return;
	}

	Super::Skill3();

	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.5f, "Skill3");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill3();
	
}

void APHWarriorCharacter::Skill4()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill4))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill4));
		return;
	}
	
	Super::Skill4();

	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill4");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
		Skill4Effect->Activate(true);
	}

	ServerRPCSkill4();
	
}

void APHWarriorCharacter::Multicast_SetActiveSkill4Effect_Implementation(bool bEffectActive)
{
	if (bEffectActive)
	{
		if (Skill4Effect && !Skill4Effect->IsActive())
		{
			Skill4Effect->Activate(true);
		}	
	}
	else
	{
		if (Skill4Effect->IsActive())
		{
			Skill4Effect->Deactivate();
		}
	}
}

void APHWarriorCharacter::ServerRPCSkill1_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill1);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);	

	StartDash();

	SendClientRPCPlayAnimation("Skill1", 1.0f);
}

void APHWarriorCharacter::ServerRPCSkill2_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill2);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.5f, "Skill2_Start");
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		StartLoopTornadoSkill();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill2_Start", 1.5f);
}

void APHWarriorCharacter::ServerRPCSkill3_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill3);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.5f, "Skill3");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3", 1.5f);
}

void APHWarriorCharacter::ServerRPCSkill4_Implementation()
{
	Super::ServerRPCSkill4_Implementation();

	StatDataComponent->IsCooldownReduction(true);
	StatDataComponent->SetCooldownReductionPercentage(2.0f);
	Skill4Effect->Activate(true);
	Multicast_SetActiveSkill4Effect(true);
	
}

void APHWarriorCharacter::StartDash()
{
	DashElapsed = 0.0f;

	DashStart = GetActorLocation();
	DashEnd = DashStart + GetActorForwardVector() * 500.0f;

	GetWorld()->GetTimerManager().SetTimer(
		DashTimerHandle,
		this,
		&APHWarriorCharacter::HandleDashStep,
		DashInterval,
		true  // 반복
	);

	
}

void APHWarriorCharacter::HandleDashStep()
{
	DashElapsed += DashInterval;
	float LerpValue = FMath::Clamp(DashElapsed / DashDuration, 0.f, 1.0f);

	// 위치 보간
	FVector NewLocation = FMath::Lerp(DashStart, DashEnd, LerpValue);
	SetActorLocation(NewLocation, true);
	
	if (LerpValue >= 1.f)
	{
		// 대쉬 끝
		GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);

		// 원하면 다시 걷기 모드로 돌아가거나 다른 처리
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void APHWarriorCharacter::StartLoopTornadoSkill()
{
	if (!bTornadoSkill)
	{
		bTornadoSkill = true;
	}

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	PlayAnimMontage(ActionMontage, 1.5f, "Skill2_Loop");

	SendClientRPCPlayAnimation("Skill2_Loop", 1.5f);
}

void APHWarriorCharacter::EndLoopTornadoSkill()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	PlayAnimMontage(ActionMontage, 1.5f, "Skill2_End");
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill2_End", 1.5f);
}

void APHWarriorCharacter::EndBerserkSkill()
{
	StatDataComponent->IsCooldownReduction(false);
	BerserkSkillRemaining = MAX_BERSERKTIME;
	if (Skill4Effect->IsActive())
	{
		Skill4Effect->Deactivate();
	}
	Multicast_SetActiveSkill4Effect(false);
}

void APHWarriorCharacter::SpawnSkill3Object()
{
	float Damage = 0.0f;
	
	if (StatDataComponent->GetBaseStat().AttackStatMap.Contains(EAttackType::Skill3))
	{
		Damage = StatDataComponent->GetBaseStat().AttackStatMap[EAttackType::Skill3].AttackDamage;
	}
	
	for (int i = 1; i <= 3; ++i)
	{
		FVector Origin = GetActorLocation();
		Origin.Z = GetCapsuleComponent()->GetComponentLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		FVector Offset = GetActorForwardVector() * i * 300.f;
		FVector SpawnLocation = Origin + Offset;

		FTimerHandle TimerHandle;
		FTimerDelegate Delegate = FTimerDelegate::CreateLambda([=, this]()
		{
			// auto* SkillObj = GetWorld()->SpawnActor<APHWarriorSkill3Object>(SkillObjClass, SpawnLocation, GetActorRotation());
			// if (SkillObj)
			// {
			// 	SkillObj->InitializeSkill(Damage, this);
			// }
			FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

			auto* SkillObj = GetWorld()->SpawnActorDeferred<APHWarriorSkill3Object>(
				SkillObjClass,
				SpawnTransform,
				this,
				this,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);

			if (SkillObj)
			{
				SkillObj->InitializeSkill(Damage, this);
				UGameplayStatics::FinishSpawningActor(SkillObj, SpawnTransform);
			}
		});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, i * 0.3f, false);
	}
}

void APHWarriorCharacter::EnableSkill1Effect(bool bActive)
{
	if (bActive)
	{
		if (Skill1Effect && !Skill1Effect->IsActive())
		{
			Skill1Effect->Activate(true);
		}	
	}
	else
	{
		if (Skill1Effect->IsActive())
		{
			Skill1Effect->Deactivate();
		}
	}
}

void APHWarriorCharacter::EnableSkill2Effect(bool bActive)
{
	if (bActive)
	{
		if (Skill2Effect && !Skill2Effect->IsActive())
		{
			Skill2Effect->Activate(true);
		}	
	}
	else
	{
		if (Skill2Effect->IsActive())
		{
			Skill2Effect->Deactivate();
		}
	}
}
