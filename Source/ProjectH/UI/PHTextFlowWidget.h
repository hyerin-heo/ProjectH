// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHTextFlowWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHTextFlowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FlowTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FullTextToDisplay;

	int32 CurrentCharacterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisplayInterval;

	FTimerHandle TextDisplayTimerHandle;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void StartTextDisplay(const FString& InText, float InInterval = 0.05f);

	void DisplayNextCharacter();

	UFUNCTION(BlueprintCallable)
	void StopTextDisplay();
};
