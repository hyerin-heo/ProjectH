// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/Component/NiagaraHitEffectComponent.h"

#include "EngineUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "Common/Common.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UNiagaraHitEffectComponent::UNiagaraHitEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNiagaraHitEffectComponent::PlayEffect(UWorld* InWorld, const FVector& HitLocation, const FVector& HitNormal,
                                            const FVector& EffectScale, AActor* DamageCauser)
{
	if (NiagaraFXToPlay && InWorld)
	{
		// HitNormal을 이용하여 이펙트 회전 설정
		FRotator EffectRotation = HitNormal.Rotation();

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			InWorld,
			NiagaraFXToPlay,
			HitLocation,
			EffectRotation,
			EffectScale,
			true, // AutoDestroy
			true // RequiredFlags
		);

		if (bShouldDealDamage)
		{
			TArray<AActor*> IgnoreActors;

			FName Tag = DamageCauser->ActorHasTag(TAG_ALLY) ? TAG_ALLY :
			DamageCauser->ActorHasTag(TAG_ENEMY) ? TAG_ENEMY : TEXT("None");

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
				HitLocation,
				DamageRadius,
				UDamageType::StaticClass(),
				IgnoreActors,
				DamageCauser,
				Cast<AController>(DamageCauser->GetInstigator()),
				false
			);
		}
	}
}
