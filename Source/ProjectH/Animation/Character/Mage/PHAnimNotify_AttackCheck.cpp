// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Mage/PHAnimNotify_AttackCheck.h"

#include "Character/PHMageCharacter.h"

void UPHAnimNotify_AttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		APHMageCharacter* AttackPawn = Cast<APHMageCharacter>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->SkillEffect(AttackType, Step);
		}
	}
}
