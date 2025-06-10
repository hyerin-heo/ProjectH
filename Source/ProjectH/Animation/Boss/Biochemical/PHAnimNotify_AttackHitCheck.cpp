// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/Biochemical/PHAnimNotify_AttackHitCheck.h"

#include "Interface/Boss/PHBossAnimationAttackInterface.h"

void UPHAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IPHBossAnimationAttackInterface* AttackPawn = Cast<IPHBossAnimationAttackInterface>(MeshComp);
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
