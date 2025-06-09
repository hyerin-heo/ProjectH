// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_Attack.h"

#include "AIController.h"
#include "PHAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTTaskNode_Attack::UBTTaskNode_Attack()
{
}

EBTNodeResult::Type UBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    
    // Check exist target in attack range
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
    if (nullptr == Target)
    {
        return EBTNodeResult::Failed;
    }

    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
    float AttackRangeWithRadius = AIPawn->GetAttackRange();
    // out of attack range
    if(DistanceToTarget > AttackRangeWithRadius)
    {
        return EBTNodeResult::Failed;
    }
    

    FAIAttackFinished AttackFinished;
    AttackFinished.BindLambda([&]()
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    });

    
    AIPawn->SetAIAttackDelegate(AttackFinished);
    AIPawn->AttackAction();
    // @PHTODO
    // 위는 서버로직이므로 클라에서 애니메이션 동기화 되도록 처리 필요.
    // 이펙트 및 UI처리도 되어야 함.

    return EBTNodeResult::InProgress;
}
