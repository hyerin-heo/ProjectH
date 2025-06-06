// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTaskNode_MoveTo_Phasable.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTaskNode_MoveTo_Phasable : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UBTTaskNode_MoveTo_Phasable();

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
