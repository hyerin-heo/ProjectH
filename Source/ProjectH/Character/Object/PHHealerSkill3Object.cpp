// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Object/PHHealerSkill3Object.h"

#include "NiagaraComponent.h"
#include "Character/Base/PHCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Subsystem/PHSoundManager.h"

// Sets default values
APHHealerSkill3Object::APHHealerSkill3Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->SetGenerateOverlapEvents(true);
	RootComponent = TriggerSphere;
	
	SkillEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SkillEffect"));
	SkillEffect->SetupAttachment(RootComponent);
	SkillEffect->SetAutoActivate(true);
	
	bReplicates = true;
}

void APHHealerSkill3Object::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &APHHealerSkill3Object::OnTriggerOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &APHHealerSkill3Object::OnTriggerOverlapEnd);
}

// Called when the game starts or when spawned
void APHHealerSkill3Object::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APHHealerSkill3Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APHHealerSkill3Object::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (!HasAuthority())
	{
		return;
	}

	APHCharacterBase* Character = Cast<APHCharacterBase>(OtherActor);
	if (Character)
	{
		Character->SetInvincibility(true);
	}
	
}

void APHHealerSkill3Object::OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority()) return;

	APHCharacterBase* Character = Cast<APHCharacterBase>(OtherActor);
	if (Character)
	{
		Character->SetInvincibility(false);
	}
}

void APHHealerSkill3Object::EnableSphereCollision(bool bActive)
{
	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer
	|| GetWorld()->GetNetMode() == NM_Standalone)
	{
		TriggerSphere->SetCollisionEnabled(bActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

		if (bActive)
		{
			if (UWorld* World = GetWorld())
			{
				if (UGameInstance* GI = World->GetGameInstance())
				{
					if (UPHSoundManager* SoundManager = GI->GetSubsystem<UPHSoundManager>())
					{
						SoundManager->PlaySFX(ESoundCategory::SFX, TEXT("HealerSkill3"), 0.7f);
					}
				}
			}
		}
	}
}

