// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHTitleWidget.h"

#include "Game/PHTitleGameMode.h"
#include "Kismet/GameplayStatics.h"

UPHTitleWidget::UPHTitleWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPHTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartBtn)
	{
		StartBtn->OnClicked.AddDynamic(this, &UPHTitleWidget::OnStartGameButtonClicked);
	}

	if (AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this))
	{
		GameModeRef = Cast<APHTitleGameMode>(GameModeBase);
	}
}

void UPHTitleWidget::OnStartGameButtonClicked()
{
	if (GameModeRef)
	{
		GameModeRef->JoinGame();
	}
}
