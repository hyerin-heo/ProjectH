// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Mage/PHAnimNotify_AttackCheck.h"

#include "Character/PHMageCharacter.h"

void UPHAnimNotify_AttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		APHMageCharacter* AttackPawn = Cast<APHMageCharacter>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			// 서버에서만 실행하도록
			if (AttackPawn->HasAuthority())
			{
				// 내부에서 복제한다.
				AttackPawn->SetSkill(AttackType, Step);	
			}
		}
	}
}
