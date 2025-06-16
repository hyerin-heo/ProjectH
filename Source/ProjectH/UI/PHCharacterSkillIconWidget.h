// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHCharacterSkillIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHCharacterSkillIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPHCharacterSkillIconWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void InitializeSkillIcon(int32 InSkillNumber, UTexture2D* NewTexture);

	void SetCoolTimeProgress(float CurrentCoolTime, float MaxCoolTime);

private:
	//스킬 이미지.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> SkillImage;

	//쿨타임프로그래스바
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> CoolTimeProgressBar;

	//단축키 표시.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InputKeyText;

	UPROPERTY()
	int32 SkillNumber;
	
	
};
