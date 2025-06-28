// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHTextFlowWidget.h"

#include "Components/TextBlock.h"

void UPHTextFlowWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentCharacterIndex = 0;
	DisplayInterval = 0.05f; // 기본값
	if (FlowTextBlock)
	{
		FlowTextBlock->SetText(FText::GetEmpty());
	}
}

void UPHTextFlowWidget::StartTextDisplay(const FString& InText, float InInterval)
{
	FullTextToDisplay = InText;
	DisplayInterval = InInterval;
	CurrentCharacterIndex = 0;

	if (FlowTextBlock)
	{
		FlowTextBlock->SetText(FText::GetEmpty()); 
	}

	GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		TextDisplayTimerHandle,
		this,
		&UPHTextFlowWidget::DisplayNextCharacter,
		DisplayInterval,
		true
	);

	DisplayNextCharacter();
}

void UPHTextFlowWidget::DisplayNextCharacter()
{
	if (!FlowTextBlock)
	{
		StopTextDisplay();
		return;
	}
	if (CurrentCharacterIndex >= FullTextToDisplay.Len())
	{
		CurrentCharacterIndex = 0;
	}

	

	FString CurrentDisplayedText = FullTextToDisplay.Left(CurrentCharacterIndex + 1);
	FlowTextBlock->SetText(FText::FromString(CurrentDisplayedText));

	CurrentCharacterIndex++;
}

void UPHTextFlowWidget::StopTextDisplay()
{
	GetWorld()->GetTimerManager().ClearTimer(TextDisplayTimerHandle);
	if (FlowTextBlock)
	{
		FlowTextBlock->SetText(FText::FromString(FullTextToDisplay));
	}
}
