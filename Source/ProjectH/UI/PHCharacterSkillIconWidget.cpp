// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHCharacterSkillIconWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UPHCharacterSkillIconWidget::UPHCharacterSkillIconWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void UPHCharacterSkillIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	CoolTimeProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("CoolTimeProgressBar")));
	ensure(CoolTimeProgressBar);

	InputKeyText = Cast<UTextBlock>(GetWidgetFromName(TEXT("InputKeyText")));
	ensure(InputKeyText);
	
	SkillImage = Cast<UImage>(GetWidgetFromName(TEXT("SkillImage")));
	ensure(SkillImage);
}

void UPHCharacterSkillIconWidget::InitializeSkillIcon(int32 InSkillNumber, UTexture2D* NewTexture)
{
	CoolTimeProgressBar->SetPercent(1.0f);
	
	switch (InSkillNumber)
	{
	case 1:
		{
			InputKeyText->SetText(FText::FromString("Q"));
			break;
		}
	case 2:
		{
			InputKeyText->SetText(FText::FromString("E"));
			break;
		}
	case 3:
		{
			InputKeyText->SetText(FText::FromString("W"));
			break;
		}
	case 4:
		{
			InputKeyText->SetText(FText::FromString("R"));
			break;
		}
	}

	SkillImage->SetBrushFromSoftTexture(NewTexture);
}

void UPHCharacterSkillIconWidget::SetCoolTimeProgress(float CurrentCoolTime, float MaxCoolTime)
{
	CoolTimeProgressBar->SetPercent(CurrentCoolTime / MaxCoolTime);
}
