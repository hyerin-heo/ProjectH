// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/GlobalEnum.h"
#include "PHCharacterSelectHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHCharacterSelectHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPHCharacterSelectHUDWidget(const FObjectInitializer& InObjectInitializer);

	FORCEINLINE EClassType GetCharacterSelectHUDClass() const{return CurrentSelectClass;}
	
	void ShowBanIcon(EClassType inClass);
	void SetSelectedCharacterIconFrameColorChange(EClassType InClassType);
	void SetAllBlock();
	
protected:
	virtual void NativeConstruct() override;
	
	void ClickedSelectIcon(EClassType inClass);
	
	//ButtonEventFunc
	UFUNCTION()
	void OnClickedSelectButton();
	UFUNCTION()
	void OnClickedStartButton();

private:
	UPROPERTY()
	TMap<EClassType, class UPHCharacterSelectIconWidget*> SelectIconWidgetMap;

	UPROPERTY()
	TObjectPtr<class UButton> SelectButton;

	UPROPERTY()
	TObjectPtr<class UButton> StartButton;
	
	UPROPERTY()
	EClassType CurrentSelectClass = EClassType::None;
	
};
