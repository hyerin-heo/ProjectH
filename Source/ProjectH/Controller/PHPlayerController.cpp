// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHPlayerController.h"

#include "UI/PHInGameHUDWidget.h"

APHPlayerController::APHPlayerController()
{
	// static ConstructorHelpers::FClassFinder<UPHInGameHUDWidget> InGameHUDWidgetRef(TEXT("/Game/ProjectH/UI/WBP_PHInGameHUD.WBP_PHInGameHUD_C"));
	//
	// if (InGameHUDWidgetRef.Class)
	// {
	// 	PHInGameHUDWidgetClass = InGameHUDWidgetRef.Class;
	// }
	
	bShowMouseCursor = true;
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
}

void APHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
