// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TurnToTarget.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	// @PHTODO Turn to target code
	return Result;
}
