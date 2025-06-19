// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/GlobalEnum.h"
#include "PHCharacterSelectIconWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickDelegate, EClassType);

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHCharacterSelectIconWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPHCharacterSelectIconWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	//Button
	UFUNCTION()
	void OnHovered();
	UFUNCTION()
	void OnUnhovered();
	UFUNCTION()
	void OnClicked();
	
public:
	void InitializeCharacterIcon(EClassType InClassType);
	
	FORCEINLINE EClassType GetClassType() const { return ClassType; }
	FORCEINLINE bool GetIsBan() const { return bIsBan; }
	
	void SetFrameColorChange(FSlateColor NewColor);
	void SetActiveBanImage();
	void CancelClicked();


protected:
	UPROPERTY()
	TObjectPtr<class UImage> CharacterImage;
	
	UPROPERTY()
	TObjectPtr<class UImage> FrameImage;
	
	UPROPERTY()
	TObjectPtr<class UImage> BanImage;

	UPROPERTY()
	TObjectPtr<class UButton> Button;

	UPROPERTY()
	EClassType ClassType = EClassType::None;

	bool bIsClicked = false;
	bool bIsBan = false;

public:
	FOnClickDelegate OnClickDelegate;
};
