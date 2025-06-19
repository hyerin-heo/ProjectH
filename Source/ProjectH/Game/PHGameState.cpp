// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHGameState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/PHCharacterSelectHUDWidget.h"

void APHGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHGameState, SelectedClassArray);
}

void APHGameState::OnRep_SelectedClassArray()
{
	
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Widgets, UPHCharacterSelectHUDWidget::StaticClass(), false);

	for (UUserWidget* Widget : Widgets)
	{
		UPHCharacterSelectHUDWidget* SelectWidget = Cast<UPHCharacterSelectHUDWidget>(Widget);
		if (SelectWidget)
		{
			APlayerState* MyPlayerState = SelectWidget->GetOwningPlayerState();
			
			for (auto& ClassInfo : SelectedClassArray)
			{
				if (ClassInfo.PlayerState != MyPlayerState)
				SelectWidget->ShowBanIcon(ClassInfo.ClassType);	
			}
		}
	}
}

void APHGameState::AddSelectedClassArray(EClassType Class, APHPlayerState* PlayerState)
{
	if (!IsClassReadySelected(Class))
	{
		SelectedClassArray.Add(FSelectedClassInfo(Class, PlayerState));
	}
}

bool APHGameState::IsClassReadySelected(EClassType Class) const
{
	for (const FSelectedClassInfo& Info : SelectedClassArray)
	{
		if (Info.ClassType == Class)
		{
			return true;
		}
	}
	
	return false;
}
