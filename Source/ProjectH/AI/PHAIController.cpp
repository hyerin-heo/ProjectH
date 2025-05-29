// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

APHAIController::APHAIController()
{
	
}

void APHAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void APHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// RunAI
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBData, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}
