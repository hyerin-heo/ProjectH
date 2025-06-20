// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHGameEndWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"

UPHGameEndWidget::UPHGameEndWidget(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UPHGameEndWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("EndButton")));
	ensure(QuitButton);

	GameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GameText")));
	ensure(GameText);

	QuitButton->OnClicked.AddDynamic(this, &UPHGameEndWidget::OnClickedQuitButton);
}

void UPHGameEndWidget::OnClickedQuitButton()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, true);
	}
}

void UPHGameEndWidget::SettingGameText(bool IsClear)
{
	if (IsClear)
	{
		GameText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
	}
	else
	{
		GameText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
	}
}
