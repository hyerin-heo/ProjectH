// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHGameMode.h"

#include "PHGameState.h"
#include "PHGameInstance.h"
#include "PHInGameLevelScriptActor.h"
#include "Controller/PHPlayerController.h"
#include "PHPlayerState.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "Character/Base/PHCharacterBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

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
	ConnectedPlayerControllers.AddUnique(NewPlayer);
}

void APHGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (Exiting)
	{
		APlayerController* Controller = Cast<APlayerController>(Exiting);
		ConnectedPlayerControllers.Remove(Controller);
		DeadPlayerControllers.Remove(Controller);
		UE_LOG(LogTemp, Log, TEXT("Player %s logged out. Remaining players: %d"), *Exiting->GetName(), ConnectedPlayerControllers.Num());
	}
}

void APHGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void APHGameMode::StartPlay()
{
	Super::StartPlay();
}

void APHGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	}
}

void APHGameMode::SetNextGame()
{
	if (CurrentStageIndex >= BossCharacterClass.Num() - 1)
	{
		StartGame();
		return;
	}
	for (APlayerController* EachPlayer : ConnectedPlayerControllers)
	{
		APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
		if (PHPlayer)
		{
			PHPlayer->ClientRPCNextGame();
		}
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

	UWorld* const World = GetWorld();
	if (!World)
	{
		return;
	}
	
	TArray<AActor*> AllPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), AllPlayerStarts);
	APlayerStart* PlayerStart = nullptr;
	if (AllPlayerStarts.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, AllPlayerStarts.Num() - 1);
		PlayerStart = Cast<APlayerStart>(AllPlayerStarts[RandomIndex]);
	}

	FVector SpawnLocation = (PlayerStart) ? PlayerStart->GetActorLocation() : FVector(-2390.000000,-130.000000,621.969011);
	FRotator SpawnRotation = (PlayerStart) ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;

	// 이미 있으면 리셋 및 포지션 다시 잡도록 세팅
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
	
	
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnInfo.Owner = PlayerControllerToSpawn;

	APHPlayerState* PlayerState = Cast<APHPlayerState>(PHPlayerController->PlayerState);
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState is not APHPlayerState."));
		return;
	}
	if (PlayerState->GetSelectedClass() == EClassType::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerState->GetSelectedClass is None."));
		return;
	}
	auto CharacterClass = CharacterClassMap.FindChecked(PlayerState->GetSelectedClass());
    APawn* NewPawn = World->SpawnActor<APawn>(CharacterClass, SpawnLocation, SpawnRotation, SpawnInfo);
    if (NewPawn)
    {
        // 서버에서 하면 클라에서도 복제됨.
        PlayerControllerToSpawn->Possess(NewPawn);
        UE_LOG(LogTemp, Log, TEXT("Player %s possessed new Pawn: %s"), *PlayerControllerToSpawn->GetName(), *NewPawn->GetName());
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
	DeadPlayerControllers.Reset();
	UPHGameInstance* Instance = Cast<UPHGameInstance>(GetWorld()->GetGameInstance());
	if (CurrentStageIndex < 0)
	{
		if (APHGameState* GS = GetGameState<APHGameState>())
		{
			GS->GameStart();

			GS->OnRep_bGameStart();
		}
		
		Instance->StartGame();
	}
	APHInGameLevelScriptActor* LevelScriptActor = Cast<APHInGameLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (LevelScriptActor)
	{
		bool CanLoad = LevelScriptActor->LoadStageLevel(++CurrentStageIndex);
		if (!CanLoad)
		{
			if (Instance)
			{
				Instance->FinishGame();
			}
			for (APlayerController* EachPlayer : ConnectedPlayerControllers)
			{
				APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
				PHPlayer->ClientRPCGameEnd(true);
			}
			return;
		}
	}

	SpawnBossCharacter();

	if (CurrentStageIndex > 0)
	{
		ResetCharacter();
	}
	else
	{
		for (APlayerController* EachPlayer : ConnectedPlayerControllers)
		{
			TrySpawnPlayerPawn(EachPlayer);
		}
	}
}

void APHGameMode::SpawnBossCharacter()
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
		// 캐릭터에서 보스 HP를 표시할 때 문제가 생김..
		ActiveBossCharacter->StopAI();
		ActiveBossCharacter->Destroy();
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	auto BossInfo = BossCharacterClass.FindChecked(CurrentStageIndex);
	// 보스 스폰
	ActiveBossCharacter = World->SpawnActor<APHBossCharacterBase>(BossInfo.BossClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (!ActiveBossCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode: Failed to spawn Boss Character."));
	}
}

void APHGameMode::CharacterDied(APlayerController* DeadCharacterController)
{
	if (DeadCharacterController)
	{
		DeadPlayerControllers.AddUnique(DeadCharacterController);
		if (DeadPlayerControllers.Num() >= ConnectedPlayerControllers.Num())
		{
			for (APlayerController* EachPlayer : ConnectedPlayerControllers)
			{
				APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
				PHPlayer->ClientRPCGameEnd(false);

				// Boss AI Stop

				if (ActiveBossCharacter)
				{
					ActiveBossCharacter->StopAI();	
				}
			}
			
			UPHGameInstance* Instance = Cast<UPHGameInstance>(GetWorld()->GetGameInstance());
			if (Instance)
			{
				Instance->FinishGame();
			}
		}
	}
}

void APHGameMode::CharacterReVive(APlayerController* ReViveCharacterController)
{
	if (DeadPlayerControllers.Contains(ReViveCharacterController))
	{
		DeadPlayerControllers.Remove(ReViveCharacterController);
	}
}

void APHGameMode::BossDied()
{
	auto BossInfo = BossCharacterClass.FindChecked(CurrentStageIndex);
	for (APlayerController* EachPlayer : ConnectedPlayerControllers)
	{
		APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
		if (PHPlayer)
		{
			PHPlayer->ClientRPCPlayBossCine(BossInfo.DiedVideoFileSource);
		}
	}
}

void APHGameMode::SetPlayerMovementState(EMovementMode Mode)
{
	for (APlayerController* EachPlayer : ConnectedPlayerControllers)
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

void APHGameMode::ResetCharacter()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	TArray<AActor*> AllPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(World, APlayerStart::StaticClass(), AllPlayerStarts);
	APlayerStart* PlayerStart = nullptr;
	if (AllPlayerStarts.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, AllPlayerStarts.Num() - 1);
		PlayerStart = Cast<APlayerStart>(AllPlayerStarts[RandomIndex]);
	}

	FVector SpawnLocation = (PlayerStart) ? PlayerStart->GetActorLocation() : FVector(-2390.000000,-130.000000,621.969011);
	FRotator SpawnRotation = (PlayerStart) ? PlayerStart->GetActorRotation() : FRotator::ZeroRotator;
	for (APlayerController* EachPlayer : ConnectedPlayerControllers)
	{
		APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
		if (PHPlayer)
		{
			PHPlayer->GetPawn()->SetActorRotation(SpawnRotation);
			PHPlayer->GetPawn()->SetActorLocation(SpawnLocation);
			PHPlayer->ClientRPCResetCharacter();
		}
	}
}

void APHGameMode::SendStartGame()
{
	for (APlayerController* EachPlayer : ConnectedPlayerControllers)
	{
		APHPlayerController* PHPlayer = Cast<APHPlayerController>(EachPlayer);
		if (PHPlayer)
		{
			PHPlayer->ClientRPCStartGame();
		}
	}
	
	if (ActiveBossCharacter)
	{
		ActiveBossCharacter->RunAI();
	}
}
