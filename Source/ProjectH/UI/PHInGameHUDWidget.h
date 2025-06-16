// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHInGameHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPHInGameHUDWidget(const FObjectInitializer& InObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;


private:
	UPROPERTY()
	TObjectPtr<class UPHHpBarWidget> HpBar;
};
