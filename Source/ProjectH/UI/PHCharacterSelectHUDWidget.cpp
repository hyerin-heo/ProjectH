// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHCharacterSelectHUDWidget.h"

#include "PHCharacterSelectIconWidget.h"
#include "Components/Button.h"
#include "Controller/PHPlayerController.h"

UPHCharacterSelectHUDWidget::UPHCharacterSelectHUDWidget(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	
}

void UPHCharacterSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int8 Type = (int8)EClassType::Warrior; Type < (int8)EClassType::End; ++Type)
	{
		const FName SelectName = *FString::Printf(TEXT("PHCharacterSelectItem_%d"), Type);

		UPHCharacterSelectIconWidget* SelectIcon = Cast<UPHCharacterSelectIconWidget>(GetWidgetFromName(SelectName));

		if (SelectIcon)
		{
			SelectIcon->InitializeCharacterIcon((EClassType)Type);
			SelectIcon->OnClickDelegate.BindUObject(this, &UPHCharacterSelectHUDWidget::ClickedSelectIcon);
			SelectIconWidgetMap.Add((EClassType)Type, SelectIcon);
		}
	}

	SelectButton = Cast<UButton>(GetWidgetFromName(TEXT("SelectButton")));
	ensure(SelectButton);
	
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));
	ensure(StartButton);

	SelectButton->OnClicked.AddDynamic(this, &UPHCharacterSelectHUDWidget::OnClickedSelectButton);
	StartButton->OnClicked.AddDynamic(this, &UPHCharacterSelectHUDWidget::OnClickedStartButton);

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (!PC->HasAuthority())
		{
			StartButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void UPHCharacterSelectHUDWidget::ClickedSelectIcon(EClassType inClass)
{
	CurrentSelectClass = inClass;

	for (auto& Icon : SelectIconWidgetMap)
	{
		if (Icon.Key == CurrentSelectClass || Icon.Value->GetIsBan()) continue;

		Icon.Value->CancelClicked();
	}
}

void UPHCharacterSelectHUDWidget::ShowBanIcon(EClassType inClass)
{
	//@PHTODO : 다른 유저가 캐릭터를 골랐을때 호출할 함수.
	for (auto& Icon : SelectIconWidgetMap)
	{
		if (Icon.Key == inClass)
		{
			Icon.Value->SetActiveBanImage();
			return;
		}
	}
}

void UPHCharacterSelectHUDWidget::SetSelectedCharacterIconFrameColorChange(EClassType InClassType)
{
	if (SelectIconWidgetMap.Contains(InClassType))
	{
		SelectIconWidgetMap[InClassType]->SetFrameColorChange(FSlateColor(FLinearColor::Red));
	}
}

void UPHCharacterSelectHUDWidget::SetAllBlock()
{
	for (auto& Icon : SelectIconWidgetMap)
	{
		Icon.Value->SetAllBlock();
	}
}

void UPHCharacterSelectHUDWidget::OnClickedSelectButton()
{
	//@PHTODO: 해당 캐릭터 골른걸 서버로 보내야한다.

	if (APlayerController* PC = GetOwningPlayer())
	{
		APHPlayerController* PlayerController = Cast<APHPlayerController>(PC);

		if (PlayerController)
		{
			PlayerController->ServerRPC_SelectCharacter(CurrentSelectClass);
		}
	}

	SelectButton->SetVisibility(ESlateVisibility::Hidden);
	SetAllBlock();
	SetSelectedCharacterIconFrameColorChange(CurrentSelectClass);
}

void UPHCharacterSelectHUDWidget::OnClickedStartButton()
{
	//@PHTODO: 시작 시켜야 한다.
}
