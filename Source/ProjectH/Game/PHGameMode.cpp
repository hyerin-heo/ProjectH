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

void APHGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	if (SkillObjectPoolSubsystem && GetWorld())
	{
		USkillObjectPoolSubsystem* MyPooledSubsystem = GetWorld()->GetSubsystem<USkillObjectPoolSubsystem>();
		if (MyPooledSubsystem && MyPooledSubsystem->GetClass() != SkillObjectPoolSubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("World Subsystem does not match configured class! Expected %s, got %s"),
				*SkillObjectPoolSubsystem->GetName(), *MyPooledSubsystem->GetClass()->GetName());
		}
	}
}

void APHGameMode::StartPlay()
{
	Super::StartPlay();
}

void APHGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
