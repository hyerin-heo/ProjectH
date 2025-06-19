// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHBossHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHBossHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPHBossHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateBossHpBar(float NewCurrentHp);

	void InitializeBossHpBar(float NewMaxHp);

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
