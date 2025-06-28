// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Tanker/PHAnimNotify_TankerAttackCheck.h"

#include "Character/PHTankerCharacter.h"

void UPHAnimNotify_TankerAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp && MeshComp->GetOwner())
	{
		APHTankerCharacter* AttackPawn = Cast<APHTankerCharacter>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			// 서버에서만 실행하도록
			if (AttackPawn->HasAuthority())
			{
				// 내부에서 복제한다.
				AttackPawn->SetSkill(AttackType);	
			}
		}
	}
}
