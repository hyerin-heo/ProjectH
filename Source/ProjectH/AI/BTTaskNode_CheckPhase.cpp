// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_CheckPhase.h"

#include "AIController.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTTaskNode_CheckPhase::UBTTaskNode_CheckPhase()
{
}

EBTNodeResult::Type UBTTaskNode_CheckPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IPHBossAIInterface* AIPawn = Cast<IPHBossAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// @PHTODO
	// phase상태일 때는 phase상태 끝난 후 callback delegate사용
	// return inprogress.
	// callback에서 return true.
	// phase상태가 아닐때는 return true.

	if (AIPawn->IsPhase())
	{
		FAIPhaseFinished PhaseFinished;
		PhaseFinished.BindLambda([&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
		AIPawn->SetAIPhaseDelegate(PhaseFinished);
		AIPawn->SpecialPattern();
		return EBTNodeResult::InProgress;

		// @PHTODO 페이즈 상태가 아니면 일반공격으로?
		// 새로 tasknode작성?
	}
	
	return EBTNodeResult::Succeeded; 
}
