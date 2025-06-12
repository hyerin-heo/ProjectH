// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Warrior/PHAN_WarriorSkill1EffectEnd.h"

#include "Character/PHWarriorCharacter.h"

void UPHAN_WarriorSkill1EffectEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHWarriorCharacter* WarriorPawn = Cast<APHWarriorCharacter>(MeshComp->GetOwner());

		if (WarriorPawn)
		{
			WarriorPawn->EnableSkill1Effect(false);
		}
	}
}
