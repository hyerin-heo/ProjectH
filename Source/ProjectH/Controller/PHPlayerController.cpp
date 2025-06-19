// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHPlayerController.h"

#include "EngineUtils.h"
#include "Boss/Base/PHBossCharacterBase.h"
#include "Game/PHGameMode.h"
#include "UI/PHCharacterSelectHUDWidget.h"
#include "UI/PHInGameHUDWidget.h"

APHPlayerController::APHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPHInGameHUDWidget> InGameHUDWidgetRef(TEXT("/Game/ProjectH/UI/WBP_PHInGameHUD.WBP_PHInGameHUD_C"));
	static ConstructorHelpers::FClassFinder<UPHCharacterSelectHUDWidget> CharacterSelectedHUDWidgetRef(TEXT("/Game/ProjectH/UI/WBP_PHCharacterSelectHUD.WBP_PHCharacterSelectHUD_C"));
	
	if (InGameHUDWidgetRef.Class)
	{
		PHInGameHUDWidgetClass = InGameHUDWidgetRef.Class;
	}
	
	if (CharacterSelectedHUDWidgetRef.Class)
	{
		PHCharacterSelectHUDWidgetClass = CharacterSelectedHUDWidgetRef.Class;
	}
	
	bShowMouseCursor = true;

	MinNetUpdateFrequency = 0.1f;
	NetUpdateFrequency=3.0f;
}

void APHPlayerController::ServerRPC_SelectCharacter_Implementation(EClassType ClassType)
{
	APHGameMode* GM = GetWorld()->GetAuthGameMode<APHGameMode>();
	if (GM)
	{
		GM->PlayerSelectCharacter(this, ClassType);
	}
}

void APHPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APHPlayerController::PostNetInit()
{
	Super::PostNetInit();
}

void APHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//마우스 클릭 이동이라 필요 없음.
	//FInputModeGameOnly GameOnlyInputMode;
	//SetInputMode(GameOnlyInputMode);

	if (!IsLocalController()) return;

	// PHInGameHUDWidget = CreateWidget<UPHInGameHUDWidget>(this, PHInGameHUDWidgetClass);
	//
	// if (PHInGameHUDWidget)
	// {
	// 	PHInGameHUDWidget->AddToViewport();
	// 	
	// 	for (TActorIterator<APHBossCharacterBase> It(GetWorld()); It; ++It)
	// 	{
	// 		PHInGameHUDWidget->InitializeBossHpBar(It->GetMaxHP());
	// 		It->OnBossHpChangedDelegate.AddUObject(PHInGameHUDWidget, &UPHInGameHUDWidget::UpdateBossHpBar);
	// 		break;  // 첫 번째 보스만 바인딩
	// 	}
	// }

	PHCharacterSelectHUDWidget = CreateWidget<UPHCharacterSelectHUDWidget>(this, PHCharacterSelectHUDWidgetClass);

	if (PHCharacterSelectHUDWidget)
	{
		PHCharacterSelectHUDWidget->AddToViewport();
	}
}

void APHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
