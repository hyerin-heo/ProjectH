// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_MoveTo_Phasable.h"

#include "AIController.h"
#include "PHAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTTaskNode_MoveTo_Phasable::UBTTaskNode_MoveTo_Phasable()
{
    bNotifyTick = true;
}

void UBTTaskNode_MoveTo_Phasable::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* ControllingPawn =  AICon ? AICon->GetPawn() : nullptr;
    if (nullptr == ControllingPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    IPHBossAIInterface* AIPawn = Cast<IPHBossAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }
    AcceptableRadius = AIPawn->GetAttackRange() * 0.5f;

    if (IPHBossAIInterface* Boss = Cast<IPHBossAIInterface>(AIPawn))
    {
        // Can move during skill cool time.
        if (!Boss->IsCoolTime())
        {
            AICon->StopMovement();
            FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
            return;
        }
    }

    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
    if (nullptr == Target)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Keep Moving
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
