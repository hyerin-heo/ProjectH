// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHCharacterSelectIconWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"

UPHCharacterSelectIconWidget::UPHCharacterSelectIconWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UPHCharacterSelectIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterImage = Cast<UImage>(GetWidgetFromName(TEXT("PHCharacterImage")));
	ensure(CharacterImage);

	FrameImage = Cast<UImage>(GetWidgetFromName(TEXT("Frame")));
	ensure(FrameImage);

	BanImage = Cast<UImage>(GetWidgetFromName(TEXT("BanImage")));
	ensure(BanImage);

	Button = Cast<UButton>(GetWidgetFromName(TEXT("SelectButton")));
	ensure(Button);

	Button->OnHovered.AddDynamic(this, &UPHCharacterSelectIconWidget::OnHovered);
	Button->OnUnhovered.AddDynamic(this, &UPHCharacterSelectIconWidget::OnUnhovered);
	Button->OnClicked.AddDynamic(this, &UPHCharacterSelectIconWidget::OnClicked);

	bIsClicked = false;
	bIsBan = false;
	bAllBlock = false;
}

void UPHCharacterSelectIconWidget::OnHovered()
{
	if (bIsClicked || bAllBlock) return;
	
	SetFrameColorChange(FSlateColor(FLinearColor::Green));
}

void UPHCharacterSelectIconWidget::OnUnhovered()
{
	if (bIsClicked || bAllBlock) return;
	
	SetFrameColorChange(FSlateColor(FLinearColor::White));
}

void UPHCharacterSelectIconWidget::OnClicked()
{
	if (bAllBlock) return;
	
	bIsClicked = true;
	OnClickDelegate.Execute(ClassType);
	SetFrameColorChange(FSlateColor(FLinearColor::Yellow));
}

void UPHCharacterSelectIconWidget::InitializeCharacterIcon(EClassType InClassType)
{
	TSoftObjectPtr<UTexture2D> Texture;
	
	switch (InClassType)
	{
	case EClassType::Warrior:
		{
			Texture = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/ProjectH/Texture/T_Worior.T_Worior")));
			break;
		}
	case EClassType::Mage:
		{
			Texture = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/ProjectH/Texture/T_Mage.T_Mage")));
			break;
		}
	case EClassType::Healer:
		{
			Texture = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/ProjectH/Texture/T_Healer.T_Healer")));
			break;
		}
	case EClassType::Tanker:
		{
			Texture = TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/ProjectH/Texture/T_Tanker.T_Tanker")));
			break;
		}
	}
	
	if (Texture.IsValid() || Texture.ToSoftObjectPath().IsValid())
	{
		UTexture2D* LoadedTexture = Texture.LoadSynchronous();
		
		if (CharacterImage && LoadedTexture)
		{
			CharacterImage->SetBrushFromTexture(LoadedTexture);
		}
	}

	ClassType = InClassType;
	BanImage->SetVisibility(ESlateVisibility::Hidden);
}

void UPHCharacterSelectIconWidget::SetFrameColorChange(FSlateColor NewColor)
{
	//FSlateColor(FLinearColor::Green)

	FrameImage->SetBrushTintColor(NewColor);
}

void UPHCharacterSelectIconWidget::SetActiveBanImage()
{
	bIsBan = true;
	BanImage->SetVisibility(ESlateVisibility::Visible);
}

void UPHCharacterSelectIconWidget::CancelClicked()
{
	bIsClicked = false;
	SetFrameColorChange(FSlateColor(FLinearColor::White));
}
