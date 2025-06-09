// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHWarriorCharacter.h"

#include "EngineUtils.h"
#include "ProjectH.h"
#include "Component/PHCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APHWarriorCharacter::APHWarriorCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ActionMontageRef(TEXT("/Game/ProjectH/Animation/Character/AM_TwoHandMontage.AM_TwoHandMontage"));
	if (ActionMontageRef.Object)
	{
		ActionMontage = ActionMontageRef.Object;
	}

	// Weapon Component
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_lSocket"));
	Weapon->SetIsReplicated(true);
	bReplicates = true;
}

void APHWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void APHWarriorCharacter::NormalAttackUI()
{
	PH_LOG(LogPHCharacter, Log, TEXT("SHOW NormalAttackUI"));
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
		SetMontageEndDelegate();	
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
		PlayAnimMontage(ActionMontage, 2.0f, "Skill1");
		SetMontageEndDelegate();
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
		PlayAnimMontage(ActionMontage, 1.0f, "Skill2_Start");
		SetMontageEndDelegate();
	}

	ServerRPCSkill2();
}

void APHWarriorCharacter::ServerRPCSkill1_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill1);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	PlayAnimMontage(ActionMontage, 2.0f, "Skill1");
	SetMontageEndDelegate();	

	StartDash();

	for (auto* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		//서버에 있는 플레이어 컨트롤러 거르기.
		if (PlayerController && GetController() != PlayerController)
		{
			//클라이언트 중에서 본인이 아닌지 확인.
			if (!PlayerController->IsLocalController())
			{
				//여기로 넘어온 플레이어 컨트롤러는 서버도 아니고, 본인 클라이언트도 아님.
				APHCharacterBase* OtherPlayer = Cast<APHCharacterBase>(PlayerController->GetPawn());
	
				if (OtherPlayer)
				{
					//Client RPC를 전송.
					OtherPlayer->ClientRPCPlayAnimation(this, "Skill1", 2.0f);
				}
			}
		}
	}
	
}

void APHWarriorCharacter::ServerRPCSkill2_Implementation()
{
	Super::ServerRPCSkill2_Implementation();

	StatDataComponent->StartSkillCooldown(EAttackType::Skill2);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill2_Start");

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &APHCharacterBase::SetActionEnd);
	if (AnimInstance)
	{
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
	}
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

	PlayAnimMontage(ActionMontage, 1.0f, "Skill2_Loop");

	
}
