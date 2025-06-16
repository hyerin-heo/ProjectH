// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/PHWidgetComponent.h"

#include "UI/PHUserWidget.h"

void UPHWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UPHUserWidget* ABUserWidget = Cast<UPHUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
