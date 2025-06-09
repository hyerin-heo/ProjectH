// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UBTTaskNode_PhasePattern.h"

#include "AIController.h"
#include "Interface/Boss/PHBossAIInterface.h"

UUBTTaskNode_PhasePattern::UUBTTaskNode_PhasePattern()
{
    
}

EBTNodeResult::Type UUBTTaskNode_PhasePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    FAIPatternAttackFinished PatternAttackFinished;
    PatternAttackFinished.BindLambda([&]                           ()
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    });
    
    AIPawn->SetAIPatternAttackDelegate(PatternAttackFinished);
    AIPawn->PhasePatternAction();
    // @PHTODO
    // 위는 서버로직이므로 클라에서 애니메이션 동기화 되도록 처리 필요.
    // 이펙트 및 UI처리도 되어야 함.

    
    return EBTNodeResult::InProgress;
}
