// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHGameMode.h"

#include "PHGameState.h"
#include "PHGameInstance.h"
#include "PHInGameLevelScriptActor.h"
#include "Controller/PHPlayerController.h"
#include "PHPlayerState.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

APHGameMode::APHGameMode()
{
	// 자동 생성 방지
	DefaultPawnClass = nullptr;
	CurrentStageIndex = -1;
}

void APHGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* APHGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void APHGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerControllers.AddUnique(NewPlayer);
}

void APHGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	if (SkillObjectPoolSubsystem && GetWorld())
	{
		USkillObjectPoolSubsystem* MyPooledSubsystem = GetWorld()->GetSubsystem<USkillObjectPoolSubsystem>();
		if (MyPooledSubsystem && MyPooledSubsystem->GetClass() != SkillObjectPoolSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("World Subsystem does not match configured class! Expected %s, got %s"),
				*SkillObjectPoolSubsystem->GetName(), *MyPooledSubsystem->GetClass()->GetName());
		}
	}
}

void APHGameMode::StartPlay()
{
	Super::StartPlay();
}

void APHGameMode::PlayerSelectCharacter(APlayerController* InPC, EClassType ClassType)
{
	APHGameState* MyGameState = GetGameState<APHGameState>();
	APHPlayerState* PlayerStat = InPC->GetPlayerState<APHPlayerState>();

	if (!MyGameState->IsClassReadySelected(ClassType))
	{
		PlayerStat->SetSelectedClass(ClassType);
		MyGameState->AddSelectedClassArray(ClassType, PlayerStat);

		MyGameState->OnRep_SelectedClassArray();
		// @PHTODO 여기서 할지 한번에 할지 고민중.
		TrySpawnPlayerPawn(InPC);
	}
}


void APHGameMode::TrySpawnPlayerPawn(APlayerController* PlayerControllerToSpawn)
{
    if (!HasAuthority())
    {
        UE_LOG(LogTemp, Error, TEXT("TrySpawnPlayerPawn called on client, should be server only!"));
        return;
    }

    if (!PlayerControllerToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("TrySpawnPlayerPawn received null PlayerController."));
        return;
    }

	// 이미 있으면 생성 안하도록
    if (PlayerControllerToSpawn->GetPawn())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player %s already has a Pawn."), *PlayerControllerToSpawn->GetName());
        return;
    }

	APHPlayerController* PHPlayerController = Cast<APHPlayerController>(PlayerControllerToSpawn);
	if (!PHPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is not APHPlayerController"));
		return;
	}

    UWorld* const World = GetWorld();
    if (!World)
    {
        return;
    }

    AActor* PlayerStart = FindPlayerStart(PlayerControllerToSpawn);
    FVector SpawnLocation = (PlayerStart) ? PlayerStart->GetActorLocation() : FVector(0.f, 0.f, 100.f);
    FRotator SpawnRotation = (PlayerStart) ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnInfo.Owner = PlayerControllerToSpawn;

	APHPlayerState* PlayerState = Cast<APHPlayerState>(PHPlayerController->PlayerState);
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not APHPlayerState."));
		return;
	}
	if (!PlayerState->GetSelectedClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState->GetSelectedClass is nullptr."));
		return;
	}
    APawn* NewPawn = World->SpawnActor<APawn>(PlayerState->GetSelectedClass(), SpawnLocation, SpawnRotation, SpawnInfo);
    if (NewPawn)
    {
        // 서버에서 하면 클라에서도 복제됨.
        PlayerControllerToSpawn->Possess(NewPawn);
        UE_LOG(LogTemp, Warning, TEXT("Player %s possessed new Pawn: %s"), *PlayerControllerToSpawn->GetName(), *NewPawn->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to spawn Pawn for player %s."), *PlayerControllerToSpawn->GetName());
    }
}

void APHGameMode::StartGame()
{
	// @PHTODO 서버가 게임 시작을 클릭 했을 때. 혹은 다음 레벨로 넘어갈 때.
	// Show Loading UI
	// 처음 입장시에만 rest call
	UPHGameInstance* Instance = Cast<UPHGameInstance>(GetWorld()->GetGameInstance());
	if (CurrentStageIndex < 0)
	{
		Instance->StartGame();
	}
	APHInGameLevelScriptActor* LevelScriptActor = Cast<APHInGameLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (LevelScriptActor)
	{
		bool CanLoad = LevelScriptActor->LoadStageLevel(++CurrentStageIndex);
		if (!CanLoad)
		{
			// @PHTODO 게임 종료.
			// 스코어창 보여주던가 타이틀레벨로 돌아가도록.
			// Hide Loading UI
			if (Instance)
			{
				Instance->FinishGame();
			}
			return;
		}
	}
	FTimerHandle LoadingUITimerHandle;
	GetWorldTimerManager().SetTimer(LoadingUITimerHandle, [&]()
	{
		// Hide Loading UI
		// Show CountDown UI
		// CountDown끝났을 시 콜백 호출.
		// SetPlayerMovementState(MOVE_Walking);
		// 보스도 움직이게 해야함.
		// SpawnBossCharacter();
		// if (ActiveBossCharacter)
		// {
		// 	ActiveBossCharacter->RunAI();
		// }
	}, 2.0f, false);
	SetPlayerMovementState(MOVE_None);
}

void APHGameMode::SpawnBossCharacter(FVector SpawnLocation, FRotator SpawnRotation)
{
	// 서버에서만 스폰 가능
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnBossCharacter called on client, should be server only!"));
		return;
	}

	if (BossCharacterClass.Num() < CurrentStageIndex + 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BossCharacterClass is not set in GameMode. Cannot spawn boss."));
		return;
	}

	UWorld* const World = GetWorld();
	if (!World)
	{
		return;
	}

	if (ActiveBossCharacter && IsValid(ActiveBossCharacter))
	{
		// 어차피 HiddenInGame하므로 삭제할 필요는 없다.
		ActiveBossCharacter->StopAI();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 보스 스폰
	ActiveBossCharacter = World->SpawnActor<APHBossCharacterBase>(BossCharacterClass[CurrentStageIndex], SpawnLocation, SpawnRotation, SpawnParams);

	if (ActiveBossCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode: Boss Character spawned: %s"), *ActiveBossCharacter->GetName());
		// @PHTODO: UI 표시 등 추가 초기화
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: Failed to spawn Boss Character."));
	}

	return;
}

void APHGameMode::SetPlayerMovementState(EMovementMode Mode)
{
	for (APlayerController* EachPlayer : PlayerControllers)
	{
		ACharacter* Character = EachPlayer->GetCharacter();
		if (Character)
		{
			UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
			MovementComp->StopMovementImmediately();
			MovementComp->SetMovementMode(Mode);
		}
	}
}

void APHGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
