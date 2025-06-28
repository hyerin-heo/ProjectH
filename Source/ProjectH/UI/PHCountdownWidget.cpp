// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHCountdownWidget.h"

#include "PHTextFlowWidget.h"
#include "Components/TextBlock.h"

void UPHCountdownWidget::NativeConstruct()
{
	Super::NativeConstruct();


	OnNativeVisibilityChanged.AddUObject(this, &UPHCountdownWidget::OnVisibilityChanged);
}

void UPHCountdownWidget::OnVisibilityChanged(ESlateVisibility InVisibility)
{
	switch (InVisibility)
	{
	case ESlateVisibility::Visible:
		{
			LoadingTextWidget->StartTextDisplay(TEXT("Loading..."));
			StartCountdown(5);
		}
		break;
	default:
		LoadingTextWidget->StopTextDisplay();
		break;
	}
}

void UPHCountdownWidget::StartCountdown(int32 InStartCount, float CountdownInterval)
{
	if (!GetWorld()) return;
	CurrentCount = InStartCount;

	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

	UpdateCountdown();

	GetWorld()->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		this,
		&UPHCountdownWidget::UpdateCountdown,
		CountdownInterval,
		true
	);
}

void UPHCountdownWidget::UpdateCountdown()
{
	if (RemainTimeText)
	{
		FText FormattedText = FText::Format(FormatString, FText::AsNumber(CurrentCount));
		RemainTimeText->SetText(FormattedText);
	}

	CurrentCount--;

	if (CurrentCount < 0)
	{
		FinishCountdown();
	}
}

void UPHCountdownWidget::FinishCountdown()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}

	if (RemainTimeText)
	{
		RemainTimeText->SetText(FText::FromString(TEXT("GO!")));
	}
	OnCountdownFinished.Broadcast();
	OnCountdownFinished.Clear();

	SetVisibility(ESlateVisibility::Collapsed);
}
