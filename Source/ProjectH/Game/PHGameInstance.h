// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PHGameInstance.generated.h"

class UPHAPIClient;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionAttemptFinished, bool, bSuccess);

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;

	virtual void Shutdown() override;

	// Title to InGame character select
	void JoinGame();

	// InGame character select to boss raid
	void StartGame();

	// InGame to Title
	void FinishGame();
	
	UFUNCTION(BlueprintCallable, Category = "Network")
	void TryConnectToServer(const FString& ServerAddress);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void HostServer();

	UPROPERTY()
	FOnConnectionAttemptFinished OnConnectionAttemptFinished;

protected:
	// Called when a new level is fully loaded and ready
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

private:

	// 연결 타임아웃 처리 함수
	void HandleConnectionTimeout();

	void DeleteCurrentRoom(const TFunction<void()>& Callback = TFunction<void()>(), bool IsShutdown = false);
	
	FTimerHandle ConnectionAttemptTimerHandle;
	FString CurrentConnectingAddress;

	UPROPERTY()
	UPHAPIClient* APIClient;

	// 현재 연결 시도 중인지 여부
	uint8 bIsAttemptingConnection:1;
	
	uint8 bIsListenServer:1;

	
	FString RoomId;
};
