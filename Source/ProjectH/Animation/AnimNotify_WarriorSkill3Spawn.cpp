// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_WarriorSkill3Spawn.h"

#include "Character/PHWarriorCharacter.h"

void UAnimNotify_WarriorSkill3Spawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHWarriorCharacter* WarriorPawn = Cast<APHWarriorCharacter>(MeshComp->GetOwner());

		if (WarriorPawn)
		{
			WarriorPawn->SpawnSkill3Object();
		}
	}
}
