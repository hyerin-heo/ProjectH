// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PHCharacterBaseAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPHCharacterBaseAnimInstance::UPHCharacterBaseAnimInstance()
{
	MovingThreshold = 3.0f;
}

void UPHCharacterBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		MovementComponent = Owner->GetCharacterMovement();
	}
}

void UPHCharacterBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
	}
}
