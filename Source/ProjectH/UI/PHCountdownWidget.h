// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHCountdownWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCountdownFinished);

class UPHTextFlowWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHCountdownWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPHTextFlowWidget> LoadingTextWidget;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RemainTimeText;

	FTimerHandle CountdownTimerHandle;

	FOnCountdownFinished OnCountdownFinished;

	virtual void NativeConstruct() override;

	void OnVisibilityChanged(ESlateVisibility InVisibility);
	
	UFUNCTION()
	void StartCountdown(int32 InStartCount, float CountdownInterval = 1.0f);

protected:
	void UpdateCountdown();

	void FinishCountdown();
	
	int CurrentCount;

	FText FormatString = FText::FromString(TEXT("{0} seconds until game starts")); 
};
