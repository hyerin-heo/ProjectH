// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/GlobalEnum.h"
#include "PHSkillBarWidget.generated.h"

/**
 * 
 */

UCLASS()
class PROJECTH_API UPHSkillBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPHSkillBarWidget(const FObjectInitializer& InObjectInitializer);

	void SetUpSkillIcons(EAttackType InAttackType, UTexture2D* NewTexture);
	void UpdateCooldownTime(EAttackType InAttackType, float CurrentCoolTime, float MaxCoolTime);

protected:
	virtual void NativeConstruct() override;

	
private:
	UPROPERTY()
	TMap<EAttackType, class UPHCharacterSkillIconWidget*> SkillIconMap;

	
};
