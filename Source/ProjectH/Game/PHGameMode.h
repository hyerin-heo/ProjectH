// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Subsystem/SkillObjectPoolSubsystem.h"
#include "PHGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APHGameMode();

	//로그인 관련 함수.
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Subsystems)
	TSubclassOf<USkillObjectPoolSubsystem> SkillObjectPoolSubsystem;
	
protected:
	virtual void PostInitializeComponents() override;
};
