// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/HitObject/PHHitEffectActor.h"

#include "EngineUtils.h"
#include "Common/Common.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APHHitEffectActor::APHHitEffectActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void APHHitEffectActor::BeginPlay()
{
	Super::BeginPlay();
	if (bShouldDealDamage)
	{
		TArray<AActor*> IgnoreActors;

		FName Tag = Owner->ActorHasTag(TAG_ALLY) ? TAG_ALLY : Owner->ActorHasTag(TAG_ENEMY) ? TAG_ENEMY : TEXT("None");

		// 월드 내 모든 액터 탐색
		for (TActorIterator<AActor> It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->Tags.Contains(Tag))
			{
				IgnoreActors.Add(Actor);
			}
		}
		UGameplayStatics::ApplyRadialDamage(
			GetWorld(),
			DamageAmount,
			GetActorLocation(),
			DamageRadius,
			UDamageType::StaticClass(),
			IgnoreActors,
			Owner,
			Owner->GetInstigatorController(),
			false
		);
		// // Owner->GetWorld()
		// UWorld* World = GetWorld();
		// if (World)
		// {
		// 	DrawDebugSphere(
		// 		World,
		// 		GetActorLocation(),
		// 		DamageRadius,
		// 		32,
		// 		FColor::Red,
		// 		false,
		// 		2.0f,
		// 		0,
		// 		1.0f
		// 	);
		// }
	}
}
