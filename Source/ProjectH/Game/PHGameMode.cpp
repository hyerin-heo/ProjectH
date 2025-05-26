// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHGameMode.h"

APHGameMode::APHGameMode()
{
}

void APHGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* APHGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void APHGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void APHGameMode::StartPlay()
{
	Super::StartPlay();
}

void APHGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
