// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/GlobalEnum.h"
#include "GameFramework/PlayerController.h"
#include "PHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APHPlayerController();

public:
	UFUNCTION(Server, Reliable)
	void ServerRPC_SelectCharacter(EClassType ClassType);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCGameEnd(bool IsClear);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCNextGame();
	
	UFUNCTION(Client, Reliable)
	void ClientRPCResetCharacter();
	
	UFUNCTION(Client, Reliable)
	void ClientRPCStartGame();

	void SetInGameHudActive(bool InGameHudActive);
	void SetInGameEndHudActive(bool InGameEndHudActive);
	void SetHiddenCharacterSelectHUD();

	void SetCountdownWidget();

	void InitWidget();

	void HostGameStart();

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPHInGameHUDWidget> PHInGameHUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPHInGameHUDWidget> PHInGameHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPHCharacterSelectHUDWidget> PHCharacterSelectHUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPHCharacterSelectHUDWidget> PHCharacterSelectHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UPHGameEndWidget> PHGameEndWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPHGameEndWidget> PHGameEndWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TSubclassOf<class UPHCountdownWidget> PHCountdownWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPHCountdownWidget> PHCountdownWidget;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TSubclassOf<class UPHNextGameWidget> PHNextGameWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UPHNextGameWidget> PHNextGameWidget;
	
	
};
