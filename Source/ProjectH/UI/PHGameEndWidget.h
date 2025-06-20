// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHGameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPHGameEndWidget(const class FObjectInitializer& ObjectInitializer);

	void IsClear(bool IsClear);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedQuitButton();
	void SettingGameText(bool IsClear);

private:
	UPROPERTY()
	TObjectPtr<class UTextBlock> GameText;
	UPROPERTY()
	TObjectPtr<class UButton> QuitButton;
	
};
