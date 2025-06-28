// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/GlobalEnum.h"
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

	void InitializeHpBar(float NewMaxHp);
	void UpdateHpBar(float NewCurrentHp);
	void UpdateMaxHp(float NewMaxHp);

	void SetUpSkillIcons(EAttackType InAttackType, UTexture2D* NewTexture);
	void UpdateCooldownTime(EAttackType InAttackType, float CurrentCoolTime, float MaxCoolTime);

	void InitializeBossHpBar(float NewMaxHp);
	void ResetBossHpBar();
	void UpdateBossHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;


private:
	UPROPERTY()
	TObjectPtr<class UPHHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UPHSkillBarWidget> SkillBar;
	UPROPERTY()
	TObjectPtr<class UPHBossHpBarWidget> BossHpBar;

	uint8 bIsInitializeBossHpBar:1;
};
