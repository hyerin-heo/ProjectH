// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Base/PHAN_CharacterAttackCheckEnd.h"

#include "Character/Base/PHCharacterBase.h"

void UPHAN_CharacterAttackCheckEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHCharacterBase* WarriorPawn = Cast<APHCharacterBase>(MeshComp->GetOwner());

		if (WarriorPawn)
		{
			WarriorPawn->EnableWeaponCollision(false);
		}
	}
}
