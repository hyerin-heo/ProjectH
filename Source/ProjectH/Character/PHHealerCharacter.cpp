// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHHealerCharacter.h"

#include "EngineUtils.h"
#include "ProjectH.h"
#include "Common/GlobalEnum.h"
#include "Net/UnrealNetwork.h"
#include "Component/PHCharacterStatComponent.h"

APHHealerCharacter::APHHealerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/Healer/AM_HealerMontage.AM_HealerMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}
	
	// Weapon Component
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
}

void APHHealerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHHealerCharacter, InvincibilityRemainingTime);
}

void APHHealerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHHealerCharacter::OnPossessed()
{
	Super::OnPossessed();
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
		float SphereRadius = 50.0f; // ← 반경 조절 가능
		

		if (GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(SphereRadius), Params))
		{
			if (Hit.GetActor()->IsA<APHCharacterBase>())
			{
				HealTargetCharacter = Cast<APHCharacterBase>(Hit.GetActor());
			}
		}
	}

	if (!HealTargetCharacter)
	{
		//@PHTODO : 인게임 허드에서 대상 못찾아줘다 보여주기.
		PH_LOG(LogPHCharacter, Log, TEXT("대상을 찾지 못했습니다."));
		HealTargetCharacter = nullptr;
		return;
	}
	
	Super::Skill1();

	PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
		ServerRPCHealTarget(HealTargetCharacter);
		HealTargetCharacter = nullptr;
		
	});
	SetMontageEndDelegate(EndDelegate);

	ServerRPCSkill1();
}

void APHHealerCharacter::SpawnNormalAttackObject()
{
	if (!HasAuthority())
		return;
	
	const FVector BaseSpawnLocation = GetActorLocation();
	const float BaseSpeed = 1000.f;
	const float BaseLifetime = 3.f;

	FRotator SpawnRotation = GetActorRotation();
	ASkillObjectBase* SkillObject = SpawnSkillObject(
		NormalAttackObject,
		BaseSpawnLocation,
		SpawnRotation
		
	);

	LaunchSkillObjectForward(SkillObject, BaseSpeed, BaseLifetime, StatDataComponent->GetDamage(EAttackType::DefaultAttack), true);
}

void APHHealerCharacter::ServerRPCHealTarget_Implementation(APHCharacterBase* InHealTargetCharacter)
{
	for (auto* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		//서버에 있는 플레이어 컨트롤러 거르기.
		if (PlayerController && GetController() != PlayerController)
		{
			APHCharacterBase* OtherPlayer = Cast<APHCharacterBase>(PlayerController->GetPawn());
	
			if (OtherPlayer == InHealTargetCharacter)
			{
				//Client RPC를 전송.
				OtherPlayer->ClientRPC_PlayerHeal(InHealTargetCharacter, StatDataComponent->GetDamage(EAttackType::Skill1));
			}
		}
	}
}
