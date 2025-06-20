// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Base/PHAN_CharacterAttackCheckStart.h"

#include "Character/Base/PHCharacterBase.h"

void UPHAN_CharacterAttackCheckStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHCharacterBase* WarriorPawn = Cast<APHCharacterBase>(MeshComp->GetOwner());

		if (WarriorPawn->HasAuthority())
		{
			UE_LOG(LogTemp, Warning, TEXT("Notify called on SERVER for: %s"), *WarriorPawn->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Notify called on CLIENT for: %s"), *WarriorPawn->GetName());
		}
		
		if (WarriorPawn)
		{
			WarriorPawn->EnableWeaponCollision(true);
		}
	}
}
