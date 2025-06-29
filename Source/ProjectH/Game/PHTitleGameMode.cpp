// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHTitleGameMode.h"

#include "PHGameInstance.h"
#include "Subsystem/PHSoundManager.h"

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

	// 1. GameInstance에서 SoundManagerSubsystem 가져오기
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
		{
			// 2. BGM 재생 (카테고리, 사운드 이름, 페이드 인 시간)
			SoundManager->PlayBGM(ESoundCategory::BGM, TEXT("Title"), 1.0f);
		}
	}
}

void APHTitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
