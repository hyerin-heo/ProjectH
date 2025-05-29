// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/PHCharacterBase.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectH.h"
#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"

// Sets default values
APHCharacterBase::APHCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// SpringArm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetUsingAbsoluteRotation(true);//스프링암의 회전이 Root컴포넌트와 상위컴포넌트회전을 따라가지않고 월드좌표계회전을 따라가도록.
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, 50.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false; //탑뷰(쿼터뷰)라 벽충돌이 없기때문에 false;
	
	// Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; //폰의 회전에 영향을 주지 않게 false.

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//캐릭터가 카메라 회전에 따라 회전되지 않게 false.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;//이동 방햐을 바라보게 하기위해 True;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 630.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;//캐릭터의 이동을 평면으로 제한.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;//시작할때 캐릭터의 위치가 평편을 벗어난 상태라면 가까운 평면을 붙혀서 시작하도록 true.
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Montage
	
	
	//Input Section.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContextObject(TEXT("/Game/ProjectH/Input/IM_PlayerBase.IM_PlayerBase"));
	if (InputMappingContextObject.Object)
	{
		InputMappingContext = InputMappingContextObject.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputMouseClickRef(TEXT("/Game/ProjectH/Input/Actions/IA_ClickMove.IA_ClickMove"));
	if (InputMouseClickRef.Object)
	{
		MouseClickMoveAction = InputMouseClickRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputMouseClickStateActionRef(TEXT("/Game/ProjectH/Input/Actions/IA_ActionClick.IA_ActionClick"));
	if (InputMouseClickStateActionRef.Object)
	{
		MouseClickStateAction = InputMouseClickStateActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputAttackActionRef(TEXT("/Game/ProjectH/Input/Actions/IA_NormalAttack.IA_NormalAttack"));
	if (InputAttackActionRef.Object)
	{
		AttackAction = InputAttackActionRef.Object;
	}
	
 	bReplicates = true;
}

void APHCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void APHCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (!InputMappingContext) return;

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	
}

//Owner가 빙의 되는 함수.(클라이언트에서는 호출이 안된다.)
void APHCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//서버의 경우 로컬 플레이어를 PossessedBy를 통해서 진행.
	UpdateMeshFromPlayerState();

	bActioning = false;
	CurrentActionType = EPlayerActionType::None;
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

	EnhancedInputComponent->BindAction(MouseClickMoveAction, ETriggerEvent::Triggered, this, &APHCharacterBase::MouseClickMove);
	EnhancedInputComponent->BindAction(MouseClickStateAction, ETriggerEvent::Triggered, this, &APHCharacterBase::SetAction);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APHCharacterBase::NormalAttackUI);
}

void APHCharacterBase::MouseClickMove()
{
	if (bActioning)
	{
		CurrentActionType = EPlayerActionType::None;
		bActioning = false;
		PH_LOG(LogPHCharacter, Log, TEXT("NormalAttack Or Skill Canceled"));
		//@PHTODO : 나중에 스킬를 눌르고 스킬 범위를 취소할때 오른쪽 눌르면 이동이 아니고 취소되게 해야한다.
		
		return;
	}
	
	APlayerController* const PC = Cast<APlayerController>(GetController());
	
	if (!PC) return;
	
	FVector WorldOrigin, WorldDirection;
	
	if (PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		FVector TraceStart =  WorldOrigin;
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.0f;
	
		FHitResult Hit;
		FCollisionQueryParams Params;
	
		Params.AddIgnoredActor(this); // 자기 자신은 무시한다.
		
	
	
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			FVector HitLocation = Hit.Location;
	
			SetNewDirection(HitLocation);
		}
	}
}

void APHCharacterBase::SetNewDirection(FVector NewDirection)
{
	float const Distance = FVector::Dist(NewDirection, GetActorLocation());

	if (Distance > 120.0f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NewDirection);
	}
}

void APHCharacterBase::SetAction()
{
	if (!bActioning) return; //아무런 행동을 안하고 있으면  return.
	
	bActioning = false;
	switch (CurrentActionType)
	{
		case EPlayerActionType::NormalAttack:
			{
				NormalAttack();
				break;
			}
			
	}
}

void APHCharacterBase::NormalAttackUI()
{
	//현재 무슨 행동중이면 return.
	if (bActioning) return;
	
	CurrentActionType = EPlayerActionType::NormalAttack;
	bActioning = true;
	PH_LOG(LogPHCharacter, Log, TEXT("SHOW Base NormalAttackUI"));
}

void APHCharacterBase::NormalAttack()
{
	PH_LOG(LogPHCharacter, Log, TEXT("Base NormalAttack!!!!"));
	
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
}

void APHCharacterBase::UpdateMeshFromPlayerState()
{
	// @PHTODO: 나중에 플레이어가 고른 캐릭터로 매쉬 로드 해야함.
	//PlayerID를 활용해서 인덱스 값 설정.()
	//int32 MeshIndex = FMath::Clamp(GetPlayerState()->GetPlayerId() % PlayerMeshes.Num(), 0, PlayerMeshes.Num() - 1);

	if (PlayerMeshes.Num() <= 0)
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Is Not PlayerCharacterMeshes"));
		return;
	}

	int32 MeshIndex = FMath::RandRange(0, PlayerMeshes.Num() - 1);
	
	
	MeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(PlayerMeshes[MeshIndex], FStreamableDelegate::CreateUObject(this, &APHCharacterBase::MeshLoadCompleted));
}

void APHCharacterBase::SetDead()
{
}

void APHCharacterBase::PlayDeadAnimation()
{
}

