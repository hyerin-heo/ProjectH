// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackInRange.h"

#include "AIController.h"
#include "PHAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return false;
    }

    IPHBossAIInterface* AIPawn = Cast<IPHBossAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return false;
    }

    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
    if (nullptr == Target)
    {
        return false;
    }

    float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
    float AttackRangeWithRadius = AIPawn->GetAttackRange();
    bResult = (DistanceToTarget <= AttackRangeWithRadius);
    return bResult;
}
