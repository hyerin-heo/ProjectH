// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PHWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void InitWidget() override;
};
