// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/Base/PHBossBaseAnimInstance.h"

#include "Boss/Base/PHBossCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

UPHBossBaseAnimInstance::UPHBossBaseAnimInstance()
{
	MovingThreshold = 3.0f;
}

void UPHBossBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<APHBossCharacterBase>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UPHBossBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
	}
}
