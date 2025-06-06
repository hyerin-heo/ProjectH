// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTaskNode_PhasePattern.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UUBTTaskNode_PhasePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UUBTTaskNode_PhasePattern();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
