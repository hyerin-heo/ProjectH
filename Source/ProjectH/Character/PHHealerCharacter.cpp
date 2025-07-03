// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHHealerCharacter.h"

#include "EngineUtils.h"
#include "NiagaraComponent.h"
#include "ProjectH.h"
#include "Common/GlobalEnum.h"
#include "Common/SkillObject/PHProjectileSkillObject.h"
#include "Net/UnrealNetwork.h"
#include "Component/PHCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Object/PHHealerSkill3Object.h"

APHHealerCharacter::APHHealerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/Healer/AM_HealerMontage.AM_HealerMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}

	static ConstructorHelpers::FClassFinder<APHHealerSkill3Object> ClassSkill3ObjectRef(TEXT("/Game/ProjectH/Blueprints/Character/SkillObject/PHHealerSkill3Object.PHHealerSkill3Object_C"));

	if (ClassSkill3ObjectRef.Class)
	{
		Skill3ObjectClass = ClassSkill3ObjectRef.Class;
	}
	
	// Weapon Component
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
}

void APHHealerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void APHHealerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInvincibilitySkill)
	{
		if (InvincibilityRemainingTime > 0.0f)
		{
			InvincibilityRemainingTime -= DeltaTime;
		}
		else
		{
			bInvincibilitySkill = false;
			InvincibilityRemainingTime = MAX_INVINCIBILITYTIME;
			EndLoopInvincibilitySkill();	
		}
	}
}

void APHHealerCharacter::OnPossessed()
{
	Super::OnPossessed();

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APHHealerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Skill3Object = GetWorld()->SpawnActor<APHHealerSkill3Object>(Skill3ObjectClass);

		if (Skill3Object)
		{
			Skill3Object->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Skill3Object->SetActorHiddenInGame(true); // 처음엔 안 보이게
			Skill3Object->EnableSphereCollision(false); // 처음엔 충돌 꺼두기		
		}
	}
}

void APHHealerCharacter::NormalAttackUI()
{
	Super::NormalAttackUI();
}

void APHHealerCharacter::NormalAttack()
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

void APHHealerCharacter::Skill1UI()
{
	Super::Skill1UI();
}

void APHHealerCharacter::Skill1()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill1))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill1));
		return;
	}

	APlayerController* const PC = Cast<APlayerController>(GetController());

	if (!PC) return;

	FVector WorldOrigin, WorldDirection;

	if (PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		FVector TraceStart = WorldOrigin;
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.0f;

		FHitResult Hit;
		FCollisionQueryParams Params;
		float SphereRadius = 120.0f; // ← 반경 조절 가능
		

		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(SphereRadius), Params))
		{
			if (Hit.GetActor()->IsA<APHCharacterBase>())
			{
				TargetCharacter = Cast<APHCharacterBase>(Hit.GetActor());
			}
		}
	}

	if (!TargetCharacter)
	{
		//@PHTODO : 인게임 허드에서 대상 못찾아줘다 보여주기.
		PH_LOG(LogPHCharacter, Log, TEXT("대상을 찾지 못했습니다."));
		TargetCharacter = nullptr;
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
	ServerRPCHealTarget(TargetCharacter);
	TargetCharacter = nullptr;
}

void APHHealerCharacter::Skill2UI()
{
	Super::Skill2UI();
}

void APHHealerCharacter::Skill2()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill2))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}

	Super::Skill2();

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

	ServerRPCSkill2();
	ServerRPCAllHealTarget();
}

void APHHealerCharacter::Skill3UI()
{
	Super::Skill3UI();
}

void APHHealerCharacter::Skill3()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill3))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill3));
		return;
	}
	
	Super::Skill3();
	
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill3Start");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
		{
			StartLoopInvincibilitySkill();
		});
		SetMontageEndDelegate(EndDelegate);	
	}

	ServerRPCSkill3();
}

void APHHealerCharacter::Skill4UI()
{
	Super::Skill4UI();
}

void APHHealerCharacter::Skill4()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill4))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"), StatDataComponent->GetSkillCooldown(EAttackType::Skill4));
		return;
	}

	APlayerController* const PC = Cast<APlayerController>(GetController());

	if (!PC) return;

	FVector WorldOrigin, WorldDirection;

	if (PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		FVector TraceStart = WorldOrigin;
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.0f;

		FHitResult Hit;
		FCollisionQueryParams Params;
		float SphereRadius = 120.0f; // ← 반경 조절 가능
		

		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(SphereRadius), Params))
		{
			if (Hit.GetActor()->IsA<APHCharacterBase>())
			{
				TargetCharacter = Cast<APHCharacterBase>(Hit.GetActor());
			}
		}
	}

	if (!TargetCharacter || !TargetCharacter->GetIsDead())
	{
		//@PHTODO : 인게임 허드에서 대상 못찾아줘다 보여주기.
		PH_LOG(LogPHCharacter, Log, TEXT("대상을 찾지 못했습니다."));
		TargetCharacter = nullptr;
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
	}

	ServerRPCSkill4();
	ServerRPCReviveTarget(TargetCharacter);
	TargetCharacter = nullptr;
}

void APHHealerCharacter::ServerRPCSkill3_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill3);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill3Start");
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		StartLoopInvincibilitySkill();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3Start", 1.0f);
}

void APHHealerCharacter::StartLoopInvincibilitySkill()
{
	if (!bInvincibilitySkill)
	{
		bInvincibilitySkill = true;
	}
	// 스킬3 이펙트 On
	Multicast_SetSkill3ObjectActive(true);
	
	PlayAnimMontage(ActionMontage, 1.0f, "Skill3Loop");

	SendClientRPCPlayAnimation("Skill3Loop", 1.0f);
}

void APHHealerCharacter::EndLoopInvincibilitySkill()
{
	PlayAnimMontage(ActionMontage, 1.0f, "Skill3End");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
		// 스킬3 이펙트 Off
		Multicast_SetSkill3ObjectActive(false);
	});
	
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3End", 1.0f);
}

void APHHealerCharacter::SpawnNormalAttackObject()
{
	if (!HasAuthority())
		return;
	
	const FVector BaseSpawnLocation = GetActorLocation();
	const float BaseSpeed = 2000.f;
	const float BaseLifetime = 3.f;

	FRotator SpawnRotation = GetActorRotation();
	APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
		NormalAttackObject,
		BaseSpawnLocation,
		SpawnRotation
		
	));

	LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, StatDataComponent->GetDamage(EAttackType::DefaultAttack), true);
}

void APHHealerCharacter::ServerRPCHealTarget_Implementation(APHCharacterBase* InHealTargetCharacter)
{
	if (InHealTargetCharacter)
	{
		InHealTargetCharacter->MulticastRPC_PlayerHeal(StatDataComponent->GetDamage(EAttackType::Skill1));
	}
}

void APHHealerCharacter::ServerRPCAllHealTarget_Implementation()
{
	if (!HasAuthority())
		return;

	const float HealAmount = StatDataComponent->GetDamage(EAttackType::Skill2);

	//모든 APHCharacterBase를 순회
	for (TActorIterator<APHCharacterBase> It(GetWorld()); It; ++It)
	{
		APHCharacterBase* OtherCharacter = *It;
	
		if (!IsValid(OtherCharacter))
			continue;
	
		// 죽은 캐릭터는 힐하지 않음
		if (OtherCharacter->GetIsDead())
			continue;
	
		OtherCharacter->MulticastRPC_AllPlayerHeal(HealAmount);
	}
}

// void APHHealerCharacter::ServerRPCReviveTarget_Implementation(APHCharacterBase* InReviveTargetCharacter)
// {
// 	if (!InReviveTargetCharacter || !InReviveTargetCharacter->GetIsDead())
// 	{
// 		return;
// 	}
//
// 	InReviveTargetCharacter->MulticastRPC_Revive();
// }

void APHHealerCharacter::Multicast_SetSkill3ObjectActive_Implementation(bool bActive)
{
	if (Skill3Object)
	{
		Skill3Object->SetActorHiddenInGame(!bActive);
		Skill3Object->EnableSphereCollision(bActive);
	}
}


