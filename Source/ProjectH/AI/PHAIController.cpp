// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PHAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

APHAIController::APHAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/ProjectH/Blueprints/Boss/Biochemical/AI/BB_BossBiochemical.BB_BossBiochemical'"));
	if (BBDataRef.Object)
	{
		BBData = BBDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/ProjectH/Blueprints/Boss/Biochemical/AI/BT_BossBiochemical.BT_BossBiochemical'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
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
	// Run
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBData, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}
