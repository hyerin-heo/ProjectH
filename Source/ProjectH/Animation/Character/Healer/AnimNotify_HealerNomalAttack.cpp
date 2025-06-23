// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Healer/AnimNotify_HealerNomalAttack.h"

#include "Character/PHHealerCharacter.h"

void UAnimNotify_HealerNomalAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHHealerCharacter* HealerPawn = Cast<APHHealerCharacter>(MeshComp->GetOwner());

		if (HealerPawn)
		{
			// 💡 서버에서만 스폰하도록 제한
			if (HealerPawn->HasAuthority())
			{
				HealerPawn->SpawnNormalAttackObject();
			}
		}
	}
}
