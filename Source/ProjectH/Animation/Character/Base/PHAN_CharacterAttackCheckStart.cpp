// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Character/Base/PHAN_CharacterAttackCheckStart.h"

#include "Character/Base/PHCharacterBase.h"
#include "Subsystem/PHSoundManager.h"

void UPHAN_CharacterAttackCheckStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		APHCharacterBase* WarriorPawn = Cast<APHCharacterBase>(MeshComp->GetOwner());
		
		
		if (WarriorPawn)
		{
			WarriorPawn->EnableWeaponCollision(true);

			// ✅ 사운드 매니저 사용
			
			if (UWorld* World = WarriorPawn->GetWorld())
			{
				if (UGameInstance* GI = World->GetGameInstance())
				{
					if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
					{
						SoundManager->PlaySFX(ESoundCategory::SFX, TEXT("SwordSound"));
					}
				}
			}
		}

		
		
	}
}
