// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckPhase.h"

#include "AIController.h"
#include "Interface/Boss/PHBossAIInterface.h"

UBTDecorator_CheckPhase::UBTDecorator_CheckPhase()
{
}

bool UBTDecorator_CheckPhase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == ControllingPawn)
    {
        return false;
    }

    IPHBossAIInterface* AIPawn = Cast<IPHBossAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return false;
    }
    
    return AIPawn->IsPhase();
}
