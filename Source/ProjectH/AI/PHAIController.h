// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PHAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHAIController : public AAIController
{
	GENERATED_BODY()
public:
	APHAIController();

	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> BBData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BTAsset;
};
