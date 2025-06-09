// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PHBossBaseAnimInstance.generated.h"

class UCharacterMovementComponent;
class APHBossCharacterBase;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHBossBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPHBossBaseAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom)
	TObjectPtr<APHBossCharacterBase> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Custom)
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float MovingThreshold;
	
};
