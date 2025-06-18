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
	// 버튼이 유효한지 확인하고 클릭 이벤트에 함수 바인딩
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UPHTitleWidget::OnStartGameButtonClicked);
	}

	if (AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this))
	{
		GameModeRef = Cast<APHTitleGameMode>(GameModeBase);
	}
}

void UPHTitleWidget::OnStartGameButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Game Button Clicked!"));

	if (GameModeRef)
	{
		GameModeRef->StartGame();
	}
}
