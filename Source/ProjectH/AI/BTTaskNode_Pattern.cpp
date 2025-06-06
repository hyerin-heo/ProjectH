// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_Pattern.h"

#include "AIController.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTTaskNode_Pattern::UBTTaskNode_Pattern()
{
}

EBTNodeResult::Type UBTTaskNode_Pattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    FAIAttackFinished AttackFinished;
    AttackFinished.BindLambda([&]()
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        AIPawn->SetCoolTime();
    });
    AIPawn->SetAIAttackDelegate(AttackFinished);
    AIPawn->PatternAction();
    return EBTNodeResult::InProgress;
}
