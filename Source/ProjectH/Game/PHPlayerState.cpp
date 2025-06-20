// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHPlayerState.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Character/PHWarriorCharacter.h"
#include "Net/UnrealNetwork.h"
#include "UI/PHCharacterSelectHUDWidget.h"

APHPlayerState::APHPlayerState()
{
	
}

void APHPlayerState::SetSelectedClass(EClassType InClassType)
{
	SelectedClass = InClassType;
	// 서버에서도 직접 UI 업데이트
	if (IsOwnedBy(GetWorld()->GetFirstPlayerController()))
	{
		OnRep_SelectedClass();
	}
}

void APHPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APHPlayerState, SelectedClass);
}

void APHPlayerState::OnRep_SelectedClass()
{
	UE_LOG(LogTemp, Warning, TEXT("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"));
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
				SelectWidget->SetAllBlock();
			}

			
			break;
		}
	}
}
