// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHMageCharacter.h"

#include "EnhancedInputComponent.h"
#include "ProjectH.h"
#include "Common/Common.h"
#include "Common/GlobalEnum.h"
#include "Common/SkillObject/PHMeshSkillObject.h"
#include "Common/SkillObject/PHProjectileSkillObject.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "Component/PHCharacterStatComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"
#include "Physics/PHCollision.h"
#include "Subsystem/SkillObjectPoolSubsystem.h"

APHMageCharacter::APHMageCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Weapon->SetIsReplicated(true);

	Skill1Component = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Skill1Effect"));
	Skill1Component->SetupAttachment(GetMesh(), TEXT("hand_rWandEnd"));
	Skill1Component->SetAutoActivate(false);

	Skill1BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill1Collision"));
	Skill1BoxComponent->SetRelativeLocation(FVector(250.0f, 0.0f, 0.0f));
	Skill1BoxComponent->SetBoxExtent(FVector(200.0f, 32.0f, 32.0f));
	Skill1BoxComponent->SetupAttachment(Skill1Component);
	Skill1BoxComponent->SetCollisionProfileName(CPROFILE_TRIGGER);

	Skill1BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APHMageCharacter::OnOverlapBegin);
	Skill1BoxComponent->OnComponentEndOverlap.AddDynamic(this, &APHMageCharacter::OnOverlapEnd);

	Skill1BoxComponent->SetIsReplicated(true);

}

void APHMageCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	if (!OtherActor)
	{
		return;
	}
	if ((OtherActor->ActorHasTag(TAG_ALLY) && this->Owner->ActorHasTag(TAG_ALLY)) || (OtherActor->ActorHasTag(TAG_ENEMY) && this->Owner->ActorHasTag(TAG_ENEMY)))
	{
		//같은편이 쏜거임.
		return;
	}
		
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherActor != this->Owner && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}

	if (OtherActor && OtherActor != this && OtherActor != this->Owner)
	{
		if (!ActiveTickDamageTargets.Contains(OtherActor))
		{
			ActiveTickDamageTargets.Add(OtherActor);
		}
		FDamageEvent InitialDamageEvent;
		OtherActor->TakeDamage(AttackDamage, InitialDamageEvent, GetInstigatorController(), this);
		// UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
		
	}
}

void APHMageCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrentActionType != EPlayerActionType::Skill1)
	{
		return;
	}
	if (!HasAuthority())
	{
		return;
	}
	if (!OtherActor)
	{
		return;
	}
	if ((OtherActor->ActorHasTag(TAG_ALLY) && this->Owner->ActorHasTag(TAG_ALLY)) || (OtherActor->ActorHasTag(TAG_ENEMY) && this->Owner->ActorHasTag(TAG_ENEMY)))
	{
		//같은편이 쏜거임.
		return;
	}
	if (ActiveTickDamageTargets.Contains(OtherActor))
	{
		ActiveTickDamageTargets.Remove(OtherActor);
	}
}

void APHMageCharacter::BeginPlay()
{
	Super::BeginPlay();
	// USkillObjectPoolSubsystem* PoolSubsystem = USkillObjectPoolSubsystem::Get(this);
	// if (!PoolSubsystem)
	// {
	// 	return;
	// }
	// for (auto SO : SkillObjectsMap)
	// {
	// 	FSkillObjectPoolData PoolData;
	// 	PoolData.PoolSize = 10;
	// 	PoolData.SkillObjectClass = SO.Value;
	// 	PoolSubsystem->InitializeSinglePool(PoolData);
	// }

	Skill1BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APHMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHMageCharacter::SetSkill(EAttackType InAttackType, uint8 InStep)
{
	float Damage = StatDataComponent->GetDamage(InAttackType);
	FVector BaseSpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.
		FindChecked(InAttackType);
	switch (InAttackType)
	{
	case EAttackType::DefaultAttack:
		{
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation,
				SpawnRotation
			));
			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, Damage, true);
		}
		break;
	case EAttackType::Skill1:
		{
		}
		break;
	case EAttackType::Skill2:
		{
			ASkillObjectBase* SkillObjectBase = SkillClass->GetDefaultObject<ASkillObjectBase>();
			float HalfHeight = SkillObjectBase->CollisionComponent->GetScaledCapsuleHalfHeight();
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation + FVector(0.0f, 0.0f, HalfHeight),
				SpawnRotation
			));
			LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, Damage, false);
		}
		break;
	case EAttackType::Skill3:
		// 텔레포트는 타이밍이 필요없음.
		break;
	case EAttackType::Skill4:
		{
			BaseSpawnLocation = CursorPosition + FVector(0.0f, 0.0f, 600.0f);
			FVector DirectionToTarget = CursorPosition - BaseSpawnLocation;
			DirectionToTarget.Normalize();
			SpawnRotation = DirectionToTarget.Rotation();
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation,
				SpawnRotation
			));
			LaunchSkillObjectForward(SkillObject, BaseSpeed, 0.5f, Damage, true);
		}
		break;
	default:
		PH_LOG(LogPHCharacter, Warning, TEXT("Invalid parameter! %s"), *ToString(InAttackType));
		break;
	}
}

void APHMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(Skill3Action, ETriggerEvent::Ongoing, this, &APHMageCharacter::Skill3UI);
}

void APHMageCharacter::TakeTickDamage()
{
	for (auto TargetActor : ActiveTickDamageTargets)
	{
		if (IsValid(TargetActor))
		{
			FDamageEvent DamageEvent;
			TargetActor->TakeDamage(AttackDamage, DamageEvent, GetInstigatorController(), this);
			PH_LOG(LogTemp, Log, TEXT("%s took %f tick damage from %s"), *TargetActor->GetName(), AttackDamage, *GetName());
		}
		else
		{
			ActiveTickDamageTargets.Remove(TargetActor);
		}
	}
}

void APHMageCharacter::ServerRPCSkill1_Implementation()
{
	//Super::ServerRPCSkill1_Implementation();
	AttackDamage = StatDataComponent->GetDamage(EAttackType::Skill1);
	StatDataComponent->StartSkillCooldown(EAttackType::Skill1);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill1");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		StartLoopLaserSkill();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill1", 1.0f);
}

void APHMageCharacter::ServerRPCSkill2_Implementation()
{
	Super::ServerRPCSkill2_Implementation();
	//override안해도 될듯
}

void APHMageCharacter::ServerRPCSkill3_Implementation()
{
	// Super::ServerRPCSkill3_Implementation();
	AttackDamage = StatDataComponent->GetDamage(EAttackType::Skill3);
	StatDataComponent->StartSkillCooldown(EAttackType::Skill3);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill3");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		TeleportTo(CursorPosition, GetActorRotation());
		SetTeleport();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3", 1.0f);
}

void APHMageCharacter::ServerRPCSkill4_Implementation()
{
	Super::ServerRPCSkill4_Implementation();
	//override안해도 될듯
}

void APHMageCharacter::NormalAttack()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
		       StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack));
		return;
	}
	Super::NormalAttack();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "NormalAttack");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCNormalAttack();
}

void APHMageCharacter::Skill1()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill1))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
		       StatDataComponent->GetSkillCooldown(EAttackType::Skill1));
		return;
	}
	Super::Skill1();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			StartLoopLaserSkill();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill1();
}

void APHMageCharacter::Skill2()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill2))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
		       StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill2();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill2();
}

void APHMageCharacter::Skill3UI()
{
	Super::Skill3UI();
	if (bUIActioning && CurrentActionType == EPlayerActionType::Skill3)
	{
		//TODO UI update		
	}
}

void APHMageCharacter::Skill3()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill3))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
		       StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill3();
	CursorPosition = GetCursorWorldPosition();
	if (!HasAuthority())
	{
		ServerRPCCursorPosition(CursorPosition);
		
		PlayAnimMontage(ActionMontage, 1.0f, "Skill3");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetTeleport();
			// SetActionEnd();
			//텔레포트하고 Skill3End실행
		});
		SetMontageEndDelegate(EndDelegate);
	}
	ServerRPCSkill3();
}

void APHMageCharacter::Skill4()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill4))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
		       StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	CursorPosition = GetCursorWorldPosition();
	Super::Skill4();
	if (!HasAuthority())
	{
		ServerRPCCursorPosition(CursorPosition);
		
		PlayAnimMontage(ActionMontage, 1.0f, "Skill4");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}
	ServerRPCSkill4();
}

void APHMageCharacter::ServerRPCCursorPosition_Implementation(FVector InCursorPosition)
{
	CursorPosition = InCursorPosition;
}

void APHMageCharacter::SetTeleport()
{
	PlayAnimMontage(ActionMontage, 1.0f, "Skill3End");
	SendClientRPCPlayAnimation("Skill3End", 1.0f);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);
}

void APHMageCharacter::StartLoopLaserSkill()
{
	PlayAnimMontage(ActionMontage, 1.0f, "Skill1Loop");
	// SendClientRPCPlayAnimation("Skill1Loop", 1.0f);
	
	Skill1Component->Activate();
	if (HasAuthority())
	{
		Skill1BoxComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		
		GetWorldTimerManager().SetTimer(TickTimerHandle, [&]()
		{
			TakeTickDamage();
		}, 0.5f, true);
	}
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [&]()
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill1End");
		// SendClientRPCPlayAnimation("Skill1End", 1.0f);
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			if (HasAuthority())
			{
				for (auto& Pair : ActiveTickDamageTargets)
				{
					GetWorldTimerManager().ClearTimer(TickTimerHandle);
				}
				ActiveTickDamageTargets.Empty();
				Skill1BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			Skill1Component->Deactivate();
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}, 5.0f, false);
}
