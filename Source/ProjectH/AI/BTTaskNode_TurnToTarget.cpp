// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TurnToTarget.h"

UBTTaskNode_TurnToTarget::UBTTaskNode_TurnToTarget()
{
}

EBTNodeResult::Type UBTTaskNode_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	// @PHTODO
	// Turn to target code
	// 클라에서 애니메이션 동기화 되도록 처리 필요.
	// 이펙트 및 UI처리도 되어야 함.
	return Result;
}
