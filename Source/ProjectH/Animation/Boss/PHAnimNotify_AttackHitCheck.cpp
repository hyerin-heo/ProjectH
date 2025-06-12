// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Boss/PHAnimNotify_AttackHitCheck.h"

#include "Interface/Boss/PHBossAnimationAttackInterface.h"

void UPHAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		ENetMode NetMode = MeshComp->GetWorld()->GetNetMode();
		if (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer || NetMode == NM_Standalone)
		{
			IPHBossAnimationAttackInterface* AttackPawn = Cast<IPHBossAnimationAttackInterface>(MeshComp->GetOwner());
			if (AttackPawn)
			{
				AttackPawn->AttackHitCheck();
			}
		}
	}
}
