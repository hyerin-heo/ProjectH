// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PHAnimNotify_PatternHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API UPHAnimNotify_PatternHitCheck : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	int32 PatternIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	uint8 Step;
};
