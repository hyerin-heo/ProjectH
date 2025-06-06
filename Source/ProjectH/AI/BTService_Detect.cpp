// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"

#include "AIController.h"
#include "PHAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/OverlapResult.h"
#include "Interface/Boss/PHBossAIInterface.h"
#include "Physics/PHCollision.h"

UBTService_Detect::UBTService_Detect()
{
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(ControllingPawn))
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	IPHBossAIInterface* AIInterface = Cast<IPHBossAIInterface>(ControllingPawn);
	if (AIInterface == nullptr)
	{
		return;
	}

	float DetectionRadius = AIInterface->GetDetectionRadius();
	

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ACTION,
		FCollisionShape::MakeSphere(DetectionRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		APawn* NearestPawn = nullptr;
		float NearestDistanceSq = FLT_MAX;
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				float DistanceSq = FVector::DistSquared(Pawn->GetActorLocation(), Center);
				if (DistanceSq < NearestDistanceSq)
				{
					NearestDistanceSq = DistanceSq;
					NearestPawn = Pawn;
				}
			}
		}

		if (NearestPawn)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, NearestPawn);
			DrawDebugSphere(World, Center, DetectionRadius, 16, FColor::Green, false, 0.1f);
			return;	
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
	DrawDebugSphere(World, Center, DetectionRadius, 16, FColor::Red, false, 0.1f);
}
