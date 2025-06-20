// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHPlayerController.h"

#include "Character/Base/PHCharacterBase.h"
#include "Game/PHGameMode.h"
#include "UI/PHCharacterSelectHUDWidget.h"
#include "UI/PHInGameHUDWidget.h"
#include "UI/PHGameEndWidget.h"

APHPlayerController::APHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPHInGameHUDWidget> InGameHUDWidgetRef(TEXT("/Game/ProjectH/UI/WBP_PHInGameHUD.WBP_PHInGameHUD_C"));
	static ConstructorHelpers::FClassFinder<UPHCharacterSelectHUDWidget> CharacterSelectedHUDWidgetRef(TEXT("/Game/ProjectH/UI/WBP_PHCharacterSelectHUD.WBP_PHCharacterSelectHUD_C"));
	static ConstructorHelpers::FClassFinder<UPHGameEndWidget>GameEndWidgetRef(TEXT("/Game/ProjectH/UI/WBP_GameEnd.WBP_GameEnd_C"));
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
	
	bShowMouseCursor = true;

	MinNetUpdateFrequency = 0.1f;
	NetUpdateFrequency=3.0f;
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
	PHGameEndWidget->IsClear(IsClear);
	SetInGameHudActive(false);
	SetInGameEndHudActive(true);
}

void APHPlayerController::SetHiddenCharacterSelectHUD()
{
	if (PHCharacterSelectHUDWidget)
	{
		PHCharacterSelectHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APHPlayerController::InitWidget()
{
	if (!IsLocalController()) return;

	PHInGameHUDWidget = CreateWidget<UPHInGameHUDWidget>(this, PHInGameHUDWidgetClass);
	PHGameEndWidget = CreateWidget<UPHGameEndWidget>(this, PHGameEndWidgetClass);
	PHCharacterSelectHUDWidget = CreateWidget<UPHCharacterSelectHUDWidget>(this, PHCharacterSelectHUDWidgetClass);
	
	// if (PHInGameHUDWidget)
	// {
	// 	PHInGameHUDWidget->AddToViewport();
	// 	
	// 	// for (TActorIterator<APHBossCharacterBase> It(GetWorld()); It; ++It)
	// 	// {
	// 	// 	PHInGameHUDWidget->InitializeBossHpBar(It->GetMaxHP());
	// 	// 	It->OnBossHpChangedDelegate.AddUObject(PHInGameHUDWidget, &UPHInGameHUDWidget::UpdateBossHpBar);
	// 	// 	break;  // 첫 번째 보스만 바인딩
	// 	// }
	// }

	
	if (PHCharacterSelectHUDWidget)
	{
		PHCharacterSelectHUDWidget->AddToViewport();
	}
	if (PHInGameHUDWidget)
	{
		PHInGameHUDWidget->AddToViewport();
	}
	if (PHGameEndWidget)
	{
		PHGameEndWidget->AddToViewport();
	}

	PHInGameHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	PHGameEndWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void APHPlayerController::SetInGameHudActive(bool InGameHudActive)
{
	if (InGameHudActive)
	{
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
