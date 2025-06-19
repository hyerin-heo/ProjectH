// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHTitleGameMode.h"

#include "PHGameInstance.h"

APHTitleGameMode::APHTitleGameMode()
{
}

void APHTitleGameMode::JoinGame()
{
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();

	UPHGameInstance* MyGI = Cast<UPHGameInstance>(GameInstance);

	if (MyGI)
	{
		MyGI->JoinGame();
	}
}

void APHTitleGameMode::BeginPlay()
{
	Super::BeginPlay();

}

void APHTitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
