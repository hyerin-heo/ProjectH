// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PHAnimNotify_TankerAttackCheck.generated.h"

enum class EAttackType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHAnimNotify_TankerAttackCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	EAttackType AttackType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	uint8 Step;
	
};
