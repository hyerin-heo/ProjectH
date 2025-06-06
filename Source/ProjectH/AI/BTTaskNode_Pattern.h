// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Pattern.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UBTTaskNode_Pattern : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_Pattern();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
