// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PHTItlePlayerController.h"

#include "Blueprint/UserWidget.h"

void APHTItlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (TitleWidgetClass && IsLocalController())
	{
		if (UUserWidget* TitleWidget = CreateWidget<UUserWidget>(this, TitleWidgetClass))
		{
			TitleWidget->AddToViewport();

			// UI 전용 입력 모드로 설정
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TitleWidget->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputModeData);
			bShowMouseCursor = true;
		}
	}
}
