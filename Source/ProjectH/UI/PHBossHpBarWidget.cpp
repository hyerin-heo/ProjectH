// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHBossHpBarWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UPHBossHpBarWidget::UPHBossHpBarWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPHBossHpBarWidget::UpdateBossHpBar(float NewCurrentHp)
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

void UPHBossHpBarWidget::InitializeBossHpBar(float NewMaxHp)
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

void UPHBossHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BossHpBar")));
	ensure(HpProgressBar);

	HpText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HpText")));
	ensure(HpText);
}
