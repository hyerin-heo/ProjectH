// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/PHCharacterBase.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectH.h"
#include "Character/Component/PHCharacterStatComponent.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"
#include "NavigationSystem.h"

// Sets default values
APHCharacterBase::APHCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetUsingAbsoluteRotation(true); //스프링암의 회전이 Root컴포넌트와 상위컴포넌트회전을 따라가지않고 월드좌표계회전을 따라가도록.
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, 50.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false; //탑뷰(쿼터뷰)라 벽충돌이 없기때문에 false;

	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; //폰의 회전에 영향을 주지 않게 false.
	
	// // Stat Component 
	StatDataComponent = CreateDefaultSubobject<UPHCharacterStatComponent>(TEXT("StatData"));

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//캐릭터가 카메라 회전에 따라 회전되지 않게 false.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; //이동 방햐을 바라보게 하기위해 True;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 630.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;   //캐릭터의 이동을 평면으로 제한.
	GetCharacterMovement()->bSnapToPlaneAtStart = true; //시작할때 캐릭터의 위치가 평편을 벗어난 상태라면 가까운 평면을 붙혀서 시작하도록 true.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


	//Input Section.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextObject(
		TEXT("/Game/ProjectH/Input/IM_PlayerBase.IM_PlayerBase"));
	if (InputMappingContextObject.Object)
	{
		InputMappingContext = InputMappingContextObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputMouseClickRef(
		TEXT("/Game/ProjectH/Input/Actions/IA_ClickMove.IA_ClickMove"));
	if (InputMouseClickRef.Object)
	{
		MouseClickMoveAction = InputMouseClickRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputMouseClickStateActionRef(
		TEXT("/Game/ProjectH/Input/Actions/IA_ActionClick.IA_ActionClick"));
	if (InputMouseClickStateActionRef.Object)
	{
		MouseClickStateAction = InputMouseClickStateActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputAttackActionRef(
		TEXT("/Game/ProjectH/Input/Actions/IA_NormalAttack.IA_NormalAttack"));
	if (InputAttackActionRef.Object)
	{
		AttackAction = InputAttackActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputEvasionActionRef(
	TEXT("/Game/ProjectH/Input/Actions/IA_Evasion.IA_Evasion"));
	if (InputEvasionActionRef.Object)
	{
		EvasionAction = InputEvasionActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkill1ActionRef(
	TEXT("/Game/ProjectH/Input/Actions/IA_Skill1.IA_Skill1"));
	if (InputSkill1ActionRef.Object)
	{
		Skill1Action = InputSkill1ActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkill2ActionRef(
	TEXT("/Game/ProjectH/Input/Actions/IA_Skill2.IA_Skill2"));
	if (InputSkill2ActionRef.Object)
	{
		Skill2Action = InputSkill2ActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkill3ActionRef(
	TEXT("/Game/ProjectH/Input/Actions/IA_Skill3.IA_Skill3"));
	if (InputSkill3ActionRef.Object)
	{
		Skill3Action = InputSkill3ActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkill4ActionRef(
	TEXT("/Game/ProjectH/Input/Actions/IA_Skill4.IA_Skill4"));
	if (InputSkill4ActionRef.Object)
	{
		Skill4Action = InputSkill4ActionRef.Object;
	}
	

	bReplicates = true;
	MeshIndex = -1;
	//SetReplicatingMovement(true);
	//SetReplicateMovement(true);
	bUIActioning = false;
	bActioning = false;
	CurrentActionType = EPlayerActionType::None;
}

void APHCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	StatDataComponent->OnHpZero.AddUObject(this, &APHCharacterBase::SetDead);
}

// Called when the game starts or when spawned
void APHCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocallyControlled() || !InputMappingContext)
	{
		return;
	}


	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

//Owner가 빙의 되는 함수.(클라이언트에서는 호출이 안된다.)
void APHCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		if (PlayerMeshes.Num() == 0)
		{
			UE_LOG(LogPHCharacter, Warning, TEXT("PlayerMeshes is Empty"));
			return;
		}

		MeshIndex = FMath::RandRange(0, PlayerMeshes.Num() - 1);

		//서버의 경우 로컬 플레이어를 PossessedBy를 통해서 진행.
		UpdateMeshFromPlayerState();
	}
}

void APHCharacterBase::OnRep_Owner()
{
	Super::OnRep_Owner();
}

void APHCharacterBase::PostNetInit()
{
	Super::PostNetInit();
}

void APHCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

float APHCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

// Called every frame
void APHCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APHCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MouseClickMoveAction, ETriggerEvent::Triggered, this,
	                                   &APHCharacterBase::MouseClickMove);
	EnhancedInputComponent->BindAction(MouseClickStateAction, ETriggerEvent::Triggered, this,
	                                   &APHCharacterBase::SetAction);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APHCharacterBase::NormalAttackUI);
	EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Triggered, this, &APHCharacterBase::Skill1UI);
	EnhancedInputComponent->BindAction(Skill2Action, ETriggerEvent::Triggered, this, &APHCharacterBase::Skill2UI);
	EnhancedInputComponent->BindAction(Skill3Action, ETriggerEvent::Triggered, this, &APHCharacterBase::Skill3UI);
	EnhancedInputComponent->BindAction(Skill4Action, ETriggerEvent::Triggered, this, &APHCharacterBase::Skill4UI);
}

void APHCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHCharacterBase, bUIActioning);
	DOREPLIFETIME(APHCharacterBase, ActionTargetRotation);
	DOREPLIFETIME(APHCharacterBase, MeshIndex);
	DOREPLIFETIME(APHCharacterBase, bActioning);
	//DOREPLIFETIME(APHCharacterBase, NormalAttackTargetRotation);
}

void APHCharacterBase::MouseClickMove()
{
	if (bUIActioning)
	{
		CurrentActionType = EPlayerActionType::None;
		bUIActioning = false;
		PH_LOG(LogPHCharacter, Log, TEXT("NormalAttack Or Skill Canceled"));
		//@PHTODO : 나중에 스킬를 눌르고 스킬 범위를 취소할때 오른쪽 눌르면 이동이 아니고 취소되게 해야한다.
	
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

		Params.AddIgnoredActor(this); // 자기 자신은 무시한다.

		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			FVector HitLocation = Hit.Location;

			SetNewLocation(HitLocation);
		}
	}
}

void APHCharacterBase::SetNewLocation(FVector NewLocation)
{
	float const Distance = FVector::Dist(NewLocation, GetActorLocation());

	if (Distance > 120.0f)
	{
		FVector SightLocation = NewLocation - GetActorLocation();
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(SightLocation).Rotator());
		ServerRPCSetNewLocation(NewLocation);
	}
}

void APHCharacterBase::ServerRPCSetNewLocation_Implementation(FVector NewLocation)
{
	//PH_LOG(LogPHCharacter, Log, TEXT("Begin"));

	float const Distance = FVector::Dist(NewLocation, GetActorLocation());
	if (Distance > 120.0f)
	{
		MulticastRPCSetNewLocation(NewLocation);
	}
}

void APHCharacterBase::MulticastRPCSetNewLocation_Implementation(FVector NewLocation)
{
	if (GetController())
	{
		PH_LOG(LogPHCharacter, Log, TEXT("TargetRotation: %d"), GetCharacterMovement()->MovementMode);
		FVector SightLocation = NewLocation - GetActorLocation();
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(SightLocation).Rotator());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NewLocation);
	}
}

void APHCharacterBase::SetAction()
{
	if (!bUIActioning || bActioning) return; //아무런 행동을 안하고 있으면  return.

	switch (CurrentActionType)
	{
	case EPlayerActionType::NormalAttack:
		{
			NormalAttack();
			break;
		}
	case EPlayerActionType::Evasion:
		{
			Evasion();
			break;
		}
	case EPlayerActionType::Skill1:
		{
			Skill1();
			break;
		}
	case EPlayerActionType::Skill2:
		{
			Skill2();
			break;
		}
	case EPlayerActionType::Skill3:
		{
			Skill3();
			break;
		}
	case EPlayerActionType::Skill4:
		{
			Skill4();
			break;
		}
	}
	
	bUIActioning = false;
}

void APHCharacterBase::SetActionEnd()
{
	bActioning = false;
	bUIActioning = false;
	PH_LOG(LogPHCharacter, Log, TEXT("SetActionEnd() bActioning : %d"), bActioning);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void APHCharacterBase::SetMontageEndDelegate(FOnMontageEnded& EndDelegate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// FOnMontageEnded EndDelegate;
	// EndDelegate.BindUObject(this, &APHCharacterBase::SetActionEnd);
	if (AnimInstance)
	{
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
	}
}


void APHCharacterBase::NormalAttackUI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::NormalAttack;
	bUIActioning = true;
}

void APHCharacterBase::NormalAttack()
{
	bActioning = true;
	RotateToCursor();
	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APHCharacterBase::EvasionUI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::Evasion;
	bUIActioning = true;
}

void APHCharacterBase::Evasion()
{
	
}

void APHCharacterBase::Skill1UI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::Skill1;
	bUIActioning = true;
}

void APHCharacterBase::Skill1()
{
	bActioning = true;
	RotateToCursor();
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// Movement Setting
	//@PHTODO: 해당 셋팅은 스킬마다 다르게 셋팅 되어야 한다.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APHCharacterBase::Skill2UI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::Skill2;
	bUIActioning = true;
}

void APHCharacterBase::Skill2()
{
	bActioning = true;
	//RotateToCursor();
	// Movement Setting
	//@PHTODO: 해당 셋팅은 스킬마다 다르게 셋팅 되어야 한다.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APHCharacterBase::Skill3UI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::Skill3;
	bUIActioning = true;
}

void APHCharacterBase::Skill3()
{
	bActioning = true;
	RotateToCursor();
	// Movement Setting
	//@PHTODO: 해당 셋팅은 스킬마다 다르게 셋팅 되어야 한다.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APHCharacterBase::Skill4UI()
{
	//현재 무슨 행동중이면 return.
	if (bUIActioning) return;

	CurrentActionType = EPlayerActionType::Skill4;
	bUIActioning = true;
}

void APHCharacterBase::Skill4()
{
	bActioning = true;
	RotateToCursor();
	// Movement Setting
	//@PHTODO: 해당 셋팅은 스킬마다 다르게 셋팅 되어야 한다.
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APHCharacterBase::ServerRPCSetActionTargetRotation_Implementation(FRotator TargetRotation)
{
	//PH_LOG(LogPHCharacter, Log, TEXT("TargetRotation: %s"), *TargetRotation.ToCompactString());

	// 서버에서 클라이언트가 공격을 위해 요청한 회전 값 설정. 
	ActionTargetRotation = TargetRotation;

	//@PHTODO : 리슨서버일때 리슨서버 클라는 OnRep이 호출이 안되서 똑같이 한번 회전을 시켜줘야한다..(나머지 클라들한테는 OnRep_ActionTargetRotation 호출)
	//데디서버로 가면 지워줘도 된다.
	if (GetController())
	{
		// 컨트롤러 회전 설정.
		GetController()->SetControlRotation(ActionTargetRotation);
	
		// 캐릭터 무브먼트의 회전 옵션을 비활성화 (캐릭터 회전을 한번에 설정하기 위해).
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		SetActorRotation(ActionTargetRotation);
	
		// 회전 설정 후 컨트롤 회전 옵션 다시 활성화.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void APHCharacterBase::OnRep_ActionTargetRotation()
{
	// 서버에서 NormalAttackTargetRotation를 변경하면, 호출되는 OnRep 함수.
	// 서버에서는 호출 안되고 클라이언트에서만 호출.
	//SetActorRotation(NormalAttackTargetRotation);
	if (GetController())
	{
		// 컨트롤러 회전 설정.
		GetController()->SetControlRotation(ActionTargetRotation);

		// 캐릭터 무브먼트의 회전 옵션을 비활성화 (캐릭터 회전을 한번에 설정하기 위해).
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		SetActorRotation(ActionTargetRotation);

		// 회전 설정 후 컨트롤 회전 옵션 다시 활성화.
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void APHCharacterBase::OnRep_MeshIndex()
{
	// 클라이언트에서만 호출되는 콜백. 
	// MeshIndex가 유효한(0 이상) 값으로 복제되었을 때 로드 시작
	if (MeshIndex >= 0 && PlayerMeshes.IsValidIndex(MeshIndex))
	{
		UpdateMeshFromPlayerState();
	}
}

void APHCharacterBase::ServerRPCNormalAttack_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::DefaultAttack);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "NormalAttack");
	
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("NormalAttack");
}

void APHCharacterBase::ServerRPCSkill1_Implementation()
{
	//그냥 스킬 애니메이션만 플레이하면 된다면 해당 함수 사용 따로 작업을 해야한다면 오버라이딩 사용.
	StatDataComponent->StartSkillCooldown(EAttackType::Skill1);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill1");
}

void APHCharacterBase::ServerRPCSkill2_Implementation()
{
	//그냥 스킬 애니메이션만 플레이하면 된다면 해당 함수 사용 따로 작업을 해야한다면 오버라이딩 사용.
	StatDataComponent->StartSkillCooldown(EAttackType::Skill2);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill2");
}

void APHCharacterBase::ServerRPCSkill3_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill3);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill3");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill3");
}

void APHCharacterBase::ServerRPCSkill4_Implementation()
{
	StatDataComponent->StartSkillCooldown(EAttackType::Skill4);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill4");

	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		SetActionEnd();
	});
	SetMontageEndDelegate(EndDelegate);

	SendClientRPCPlayAnimation("Skill4");
}


void APHCharacterBase::ClientRPCPlayAnimation_Implementation(APHCharacterBase* CharacterPlayer, FName ActionName,float AnimSpeed)
{
	if (CharacterPlayer)
	{
		CharacterPlayer->PlayAnimMontage(ActionMontage, AnimSpeed, ActionName);
	}
}

void APHCharacterBase::MeshLoadCompleted()
{
	if (MeshHandle.IsValid())
	{
		USkeletalMesh* NewMesh = Cast<USkeletalMesh>(MeshHandle->GetLoadedAsset());
		if (NewMesh)
		{
			GetMesh()->SetSkeletalMesh(NewMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	MeshHandle->ReleaseHandle();
	MeshHandle.Reset();
}

void APHCharacterBase::UpdateMeshFromPlayerState()
{
	// @PHTODO: 나중에 플레이어가 고른 캐릭터로 매쉬 로드 해야함.
	//PlayerID를 활용해서 인덱스 값 설정.()
	//int32 MeshIndex = FMath::Clamp(GetPlayerState()->GetPlayerId() % PlayerMeshes.Num(), 0, PlayerMeshes.Num() - 1);

	if (!PlayerMeshes.IsValidIndex(MeshIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MeshIndex: %d"), MeshIndex);
		return;
	}

	// 만약 이미 로드 요청이 남아 있다면 해제(중복 방지)
	if (MeshHandle.IsValid())
	{
		MeshHandle->ReleaseHandle();
		MeshHandle.Reset();
	}

	// TSoftObjectPtr 에서 Load 요청
	MeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		PlayerMeshes[MeshIndex],FStreamableDelegate::CreateUObject(this, &APHCharacterBase::MeshLoadCompleted));
}

void APHCharacterBase::RotateToCursor()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	// 1) 화면 좌표 → 월드공간 광선(Origin + Direction) 변환
	FVector RayOrigin, RayDir;
	if (PC->DeprojectMousePositionToWorld(RayOrigin, RayDir))
	{
		// 2) 캐릭터 Z 높이의 수평면과 광선 교차점 계산
		float CharacterZ = GetActorLocation().Z;
		if (!FMath::IsNearlyZero(RayDir.Z))
		{
			float HitDistance = (CharacterZ - RayOrigin.Z) / RayDir.Z; //파라메트릭 식
			FVector TargetPoint = RayOrigin + RayDir * HitDistance;

			// 3) 회전 계산
			FVector ToTarget = TargetPoint - GetActorLocation();
			ToTarget.Z = 0.f;
			if (!ToTarget.IsNearlyZero())
			{
				//FRotator NewRot = ToTarget.Rotation();
				ServerRPCSetActionTargetRotation(ToTarget.Rotation());
				//SetActorRotation(FRotator(0.f, NewRot.Yaw, 0.f));
			}
		}
	}
}


void APHCharacterBase::SetDead()
{
}

void APHCharacterBase::SendClientRPCPlayAnimation(FName SectionName, float AnimSpeed)
{
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
					OtherPlayer->ClientRPCPlayAnimation(this, SectionName, AnimSpeed);
				}
			}
		}
	}
}

void APHCharacterBase::PlayDeadAnimation()
{
}
