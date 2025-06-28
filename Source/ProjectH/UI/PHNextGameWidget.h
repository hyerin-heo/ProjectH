// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHNextGameWidget.generated.h"

class UPHTextFlowWidget;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHNextGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPHTextFlowWidget> TextFlowWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> NextStageButton;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickNextStageButton();

	void OnVisibilityChanged(ESlateVisibility InVisibility) const;
};
