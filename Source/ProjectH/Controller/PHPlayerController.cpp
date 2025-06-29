// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHPlayerController.h"

#include "EngineUtils.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "Character/Base/PHCharacterBase.h"
#include "Game/PHGameMode.h"
#include "Subsystem/PHSoundManager.h"
#include "UI/PHCharacterSelectHUDWidget.h"
#include "UI/PHCountdownWidget.h"
#include "UI/PHInGameHUDWidget.h"
#include "UI/PHGameEndWidget.h"
#include "UI/PHNextGameWidget.h"
#include "UI/PHVideoWidget.h"

APHPlayerController::APHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPHInGameHUDWidget> InGameHUDWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_PHInGameHUD.WBP_PHInGameHUD_C"));
	static ConstructorHelpers::FClassFinder<UPHCharacterSelectHUDWidget> CharacterSelectedHUDWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_PHCharacterSelectHUD.WBP_PHCharacterSelectHUD_C"));
	static ConstructorHelpers::FClassFinder<UPHGameEndWidget> GameEndWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_GameEnd.WBP_GameEnd_C"));

	static ConstructorHelpers::FClassFinder<UPHCountdownWidget> CountdownWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_Countdown.WBP_Countdown_C"));

	static ConstructorHelpers::FClassFinder<UPHNextGameWidget> NextGameWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_NextStageWidget.WBP_NextStageWidget_C"));

	static ConstructorHelpers::FClassFinder<UPHVideoWidget> VideoWidgetRef(
		TEXT("/Game/ProjectH/UI/WBP_VideoWidget.WBP_VideoWidget_C"));
	if (InGameHUDWidgetRef.Class)
	{
		PHInGameHUDWidgetClass = InGameHUDWidgetRef.Class;
	}

	if (CharacterSelectedHUDWidgetRef.Class)
	{
		PHCharacterSelectHUDWidgetClass = CharacterSelectedHUDWidgetRef.Class;
	}

	if (GameEndWidgetRef.Class)
	{
		PHGameEndWidgetClass = GameEndWidgetRef.Class;
	}

	if (CountdownWidgetRef.Class)
	{
		PHCountdownWidgetClass = CountdownWidgetRef.Class;
	}

	if (NextGameWidgetRef.Class)
	{
		PHNextGameWidgetClass = NextGameWidgetRef.Class;
	}

	if (VideoWidgetRef.Class)
	{
		PHVideoWidgetClass = VideoWidgetRef.Class;
	}

	bShowMouseCursor = true;
	

	MinNetUpdateFrequency = 0.1f;
	NetUpdateFrequency = 3.0f;
}

void APHPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APHPlayerController::PostNetInit()
{
	Super::PostNetInit();
}


void APHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	APHCharacterBase* CharacterBase = Cast<APHCharacterBase>(InPawn);
	if (CharacterBase)
	{
		CharacterBase->OnPossessed();
		CurrentMouseCursor = EMouseCursor::Default;
	}
}

void APHPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void APHPlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitWidget();
	//마우스 클릭 이동이라 필요 없음.
	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);
	// 1. GameInstance에서 SoundManagerSubsystem 가져오기
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
		{
			// 2. BGM 재생 (카테고리, 사운드 이름, 페이드 인 시간)
			SoundManager->PlayBGM(ESoundCategory::BGM, TEXT("CharacterSelect"), 1.0f);
		}
	}
}

void APHPlayerController::ServerRPC_SelectCharacter_Implementation(EClassType ClassType)
{
	APHGameMode* GM = GetWorld()->GetAuthGameMode<APHGameMode>();
	if (GM)
	{
		GM->PlayerSelectCharacter(this, ClassType);
	}
}

void APHPlayerController::ClientRPCGameEnd_Implementation(bool IsClear)
{
	SetInGameHudActive(false);
	SetInGameEndHudActive(true);
	PHGameEndWidget->IsClear(IsClear);
}

void APHPlayerController::SetHiddenCharacterSelectHUD()
{
	if (PHCharacterSelectHUDWidget)
	{
		PHCharacterSelectHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APHPlayerController::SetCountdownWidget()
{
	if (PHCountdownWidget)
	{
		PHCountdownWidget->SetVisibility(ESlateVisibility::Visible);
		if (HasAuthority())
		{
			PHCountdownWidget->OnCountdownFinished.AddLambda([&]()
			{
				if (HasAuthority())
				{
					APHGameMode* GM = GetWorld()->GetAuthGameMode<APHGameMode>();
					if (GM)
					{
						GM->SendStartGame();
					}
				}
			});
		}
	}
}

void APHPlayerController::InitWidget()
{
	if (!IsLocalController()) return;

	// PHInGameHUDWidget = CreateWidget<UPHInGameHUDWidget>(this, PHInGameHUDWidgetClass);
	// PHGameEndWidget = CreateWidget<UPHGameEndWidget>(this, PHGameEndWidgetClass);
	PHCharacterSelectHUDWidget = CreateWidget<UPHCharacterSelectHUDWidget>(this, PHCharacterSelectHUDWidgetClass);
	//
	//
	if (PHCharacterSelectHUDWidget)
	{
		PHCharacterSelectHUDWidget->AddToViewport();
	}
	// if (PHInGameHUDWidget)
	// {
	// 	PHInGameHUDWidget->AddToViewport();
	// }
	// if (PHGameEndWidget)
	// {
	// 	PHGameEndWidget->AddToViewport();
	// }

	// PHInGameHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	// PHGameEndWidget->SetVisibility(ESlateVisibility::Collapsed);


	PHNextGameWidget = CreateWidget<UPHNextGameWidget>(this, PHNextGameWidgetClass);
	if (PHNextGameWidget)
	{
		PHNextGameWidget->AddToViewport(100);
		PHNextGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PHCountdownWidget = CreateWidget<UPHCountdownWidget>(this, PHCountdownWidgetClass);
	if (PHCountdownWidget)
	{
		PHCountdownWidget->AddToViewport(100);
		PHCountdownWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	PHVideoWidget = CreateWidget<UPHVideoWidget>(this, PHVideoWidgetClass);
	if (PHVideoWidget)
	{
		PHVideoWidget->AddToViewport(200);
		PHVideoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APHPlayerController::ClientRPCNextGame_Implementation()
{
	SetInGameHudActive(false);
	PHNextGameWidget->SetVisibility(ESlateVisibility::Visible);
}

void APHPlayerController::ClientRPCResetCharacter_Implementation()
{
	PHNextGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	APHCharacterBase* CharacterBase = Cast<APHCharacterBase>(GetPawn());
	if (CharacterBase)
	{
		CharacterBase->Init();
	}
	PHCountdownWidget->SetVisibility(ESlateVisibility::Visible);
}

void APHPlayerController::ClientRPCStartGame_Implementation()
{
	APHCharacterBase* CharacterBase = Cast<APHCharacterBase>(GetPawn());
	if (CharacterBase)
	{
		CharacterBase->Start();
	}

	for (TActorIterator<APHBossCharacterBase> It(GetWorld()); It; ++It)
	{
		if (It->GetHP() > KINDA_SMALL_NUMBER)
		{
			PHInGameHUDWidget->InitializeBossHpBar(It->GetMaxHP());
			It->OnBossHpChangedDelegate.AddUObject(PHInGameHUDWidget, &UPHInGameHUDWidget::UpdateBossHpBar);
			break; // HP가 0보다 큰 보스만 바인딩
		}
	}
}

void APHPlayerController::ClientRPCPlayBossCine_Implementation(UFileMediaSource* VideoFileSource)
{
	if (PHVideoWidget)
	{
		PHVideoWidget->SetVisibility(ESlateVisibility::Visible);
		PHVideoWidget->OnFinishCinematic.AddLambda([&]()
		{
			PHVideoWidget->SetVisibility(ESlateVisibility::Collapsed);
			if (HasAuthority())
			{
				APHGameMode* GM = GetWorld()->GetAuthGameMode<APHGameMode>();
				if (GM)
				{
					GM->SetNextGame();
				}
			}
		});
		PHVideoWidget->SetMediaSource(VideoFileSource);
	}
}

void APHPlayerController::SetInGameHudActive(bool InGameHudActive)
{
	if (InGameHudActive)
	{
		if (!PHInGameHUDWidget)
		{
			PHInGameHUDWidget = CreateWidget<UPHInGameHUDWidget>(this, PHInGameHUDWidgetClass);
			if (PHInGameHUDWidget)
			{
				PHInGameHUDWidget->AddToViewport();
			}
		}

		PHInGameHUDWidget->ResetBossHpBar();
		PHInGameHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PHInGameHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APHPlayerController::SetInGameEndHudActive(bool InGameEndHudActive)
{
	if (InGameEndHudActive)
	{
		PHGameEndWidget = CreateWidget<UPHGameEndWidget>(this, PHGameEndWidgetClass);

		if (PHGameEndWidget)
		{
			PHGameEndWidget->AddToViewport();
		}

		PHGameEndWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		PHGameEndWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APHPlayerController::HostGameStart()
{
	if (HasAuthority())
	{
		APHGameMode* GM = GetWorld()->GetAuthGameMode<APHGameMode>();
		if (GM)
		{
			GM->StartGame();
		}
	}
}
