// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Warrior/AnimNotify_WarriorSkill3Spawn.h"

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
			// 💡 서버에서만 스폰하도록 제한
			if (WarriorPawn->HasAuthority())
			{
				UE_LOG(LogTemp, Warning, TEXT("Notify 실행: HasAuthority=%d, IsLocallyControlled=%d, Name=%s"),
					WarriorPawn->HasAuthority(), WarriorPawn->IsLocallyControlled(), *WarriorPawn->GetName());
				
				WarriorPawn->SpawnSkill3Object();
			}
		}
	}
}
