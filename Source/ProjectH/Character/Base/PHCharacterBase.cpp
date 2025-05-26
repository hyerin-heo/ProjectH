// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Base/PHCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
APHCharacterBase::APHCharacterBase()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
 	bReplicates = true;
}

void APHCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void APHCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APHCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APHCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void APHCharacterBase::MouseClickMove()
{
	APlayerController* const PC = Cast<APlayerController>(GetController());
	
	if (!PC) return;
	
	FVector WorldOrigin, WorldDirection;
	
	if (PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection))
	{
		FVector TraceStart =  WorldOrigin;
		FVector TraceEnd = WorldOrigin + WorldDirection * 10000.0f;
	
		FHitResult Hit;
		FCollisionQueryParams Params;
	
		Params.AddIgnoredActor(this); // 자기 자신은 무시한다.
	
	
		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params))
		{
			FVector HitLocation = Hit.Location;
	
			SetNewDirection(HitLocation);
		}
	}
}

void APHCharacterBase::SetNewDirection(FVector NewDirection)
{
	float const Distance = FVector::Dist(NewDirection, GetActorLocation());

	if (Distance > 120.0f)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), NewDirection);
	}
}

