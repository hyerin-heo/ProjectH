// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHNextGameWidget.h"

#include "PHTextFlowWidget.h"
#include "Components/Button.h"
#include "Controller/PHPlayerController.h"

void UPHNextGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (PC->HasAuthority())
		{
			NextStageButton->OnClicked.AddDynamic(this, &UPHNextGameWidget::OnClickNextStageButton);
			NextStageButton->SetVisibility(ESlateVisibility::Visible);
		}else
		{
			NextStageButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	OnNativeVisibilityChanged.AddUObject(this, &UPHNextGameWidget::OnVisibilityChanged);
}

void UPHNextGameWidget::OnClickNextStageButton()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		APHPlayerController* PlayerController = Cast<APHPlayerController>(PC);
		
		if (PlayerController && PlayerController->HasAuthority())
		{
			PlayerController->HostGameStart();
		}
	}
}

void UPHNextGameWidget::OnVisibilityChanged(ESlateVisibility InVisibility) const
{
	switch (InVisibility) {
	case ESlateVisibility::Visible:
		{
			if (APlayerController* PC = GetOwningPlayer())
			{
				if (PC->HasAuthority())
				{
					TextFlowWidget->StartTextDisplay(TEXT("Would you like to proceed to the next step?"));
				}else
				{
					TextFlowWidget->StartTextDisplay(TEXT("Waiting for the host's choice."));
				}
			}
		}
		break;
	default:
		TextFlowWidget->StopTextDisplay();
		break;
	}
}
