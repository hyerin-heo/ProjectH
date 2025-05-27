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
}

void APHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}
