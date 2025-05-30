// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHPlayerController.h"

APHPlayerController::APHPlayerController()
{
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
