// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHPlayerState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/PHCharacterSelectHUDWidget.h"

APHPlayerState::APHPlayerState()
{
	
}

void APHPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHPlayerState, SelectedClass);
}

void APHPlayerState::OnRep_SelectedClass()
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UPHCharacterSelectHUDWidget::StaticClass(), false);

	for (UUserWidget* Widget : Widgets)
	{
		UPHCharacterSelectHUDWidget* SelectWidget = Cast<UPHCharacterSelectHUDWidget>(Widget);
		if (SelectWidget)
		{
			if (SelectWidget->GetCharacterSelectHUDClass() == SelectedClass)
			{
				SelectWidget->SetSelectedCharacterIconFrameColorChange(SelectedClass);
			}
			else
			{
				SelectWidget->ShowBanIcon(SelectedClass);	
			}
			break;
		}
	}
}
