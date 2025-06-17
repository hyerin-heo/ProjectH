// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/Character/PHCharacterWidgetInterface.h"

UPHHpBarWidget::UPHHpBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UPHHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpText")));
	ensure(HpText);

	IPHCharacterWidgetInterface* CharacterWidget = Cast<IPHCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UPHHpBarWidget::UpdateMaxHp(float NewMaxHp)
{
	MaxHp = NewMaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpText)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp)));
	}
}


void UPHHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f);
	
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpText)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp)));
	}
}

void UPHHpBarWidget::InitializeHpBar(float NewMaxHp)
{
   	MaxHp = NewMaxHp;
	CurrentHp = MaxHp;

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpText)
	{
		HpText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurrentHp, MaxHp)));
	}
}
