// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PHTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHTitleWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPHTitleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UFUNCTION()
	void OnStartGameButtonClicked();

	UPROPERTY()
	class APHTitleGameMode* GameModeRef;
	
};
