// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHTitleGameMode.h"

#include "PHGameInstance.h"
#include "ProjectH.h"
#include "SocketSubsystem.h"
#include "API/PHAPIClient.h"
#include "Blueprint/UserWidget.h"
#include "Common/GlobalEnum.h"
#include "Interfaces/IHttpResponse.h"

APHTitleGameMode::APHTitleGameMode()
{
}

void APHTitleGameMode::StartGame()
{
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();

	UPHGameInstance* MyGI = Cast<UPHGameInstance>(GameInstance);

	if (MyGI)
	{
		MyGI->StartGame();
	}
}

void APHTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void APHTitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (TitleWidgetClass)
	{
		if (UUserWidget* TitleWidget = CreateWidget<UUserWidget>(NewPlayer, TitleWidgetClass))
		{
			TitleWidget->AddToViewport();

			// UI 전용 입력 모드로 설정
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TitleWidget->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			NewPlayer->SetInputMode(InputModeData);
			NewPlayer->bShowMouseCursor = true;
		}
	}
}
