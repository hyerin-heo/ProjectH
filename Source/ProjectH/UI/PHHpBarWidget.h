// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PHUserWidget.h"
#include "PHHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHHpBarWidget : public UPHUserWidget
{
	GENERATED_BODY()
	
public:
	UPHHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateHpBar(float NewCurrentHp);
	void UpdateMaxHp(float NewMaxHp);

	void InitializeHpBar(float NewMaxHp);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpText;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
	
};
