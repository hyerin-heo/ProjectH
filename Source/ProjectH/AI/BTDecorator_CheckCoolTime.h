// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckCoolTime.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTDecorator_CheckCoolTime : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckCoolTime();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
