// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHPlayerState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PHWarriorCharacter.h"
#include "Net/UnrealNetwork.h"
#include "UI/PHCharacterSelectHUDWidget.h"

APHPlayerState::APHPlayerState()
{
	
}

UClass* APHPlayerState::GetSelectedClass() const
{
	switch (SelectedClass)
	{
	case EClassType::Warrior:
		return APHWarriorCharacter::StaticClass();
	case EClassType::Healer:
		// @PHTODO 추후 작업할 것
		return APHWarriorCharacter::StaticClass();
	case EClassType::Mage:
		// @PHTODO 추후 작업할 것
		return APHWarriorCharacter::StaticClass();
	case EClassType::Tanker:
		// @PHTODO 추후 작업할 것
		return APHWarriorCharacter::StaticClass();
	default:
		return nullptr;
	}
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
