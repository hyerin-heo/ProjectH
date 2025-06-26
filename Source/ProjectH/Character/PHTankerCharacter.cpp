// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHTankerCharacter.h"

#include "ProjectH.h"
#include "Common/HitObject/PHHitEffectActor.h"
#include "Common/SkillObject/PHProjectileSkillObject.h"
#include "Component/PHCharacterStatComponent.h"
#include "Physics/PHCollision.h"

APHTankerCharacter::APHTankerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetCollisionProfileName(CPROFILE_TRIGGER);
	Shield->SetupAttachment(GetMesh(), TEXT("LeftWeaponShield"));
	Weapon->SetIsReplicated(true);
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void APHTankerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APHTankerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHTankerCharacter::SetSkill(EAttackType InAttackType, uint8 Step)
{
	const float InitialSpeed = 1500.0f;
	const float LifeTime = 2.0f;
	float Damage = StatDataComponent->GetDamage(InAttackType);
	FVector BaseSpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	const TSubclassOf<ASkillObjectBase>& SkillClass = SkillObjectsMap.
		FindChecked(InAttackType);
	switch (InAttackType)
	{
	case EAttackType::DefaultAttack:
		break;
	case EAttackType::Skill1:
		break;
	case EAttackType::Skill2:
		break;
	case EAttackType::Skill3:
		{
			FTransform SpawnTransform = GetMesh()->GetSocketTransform(TEXT("HammerSlash"));
			BaseSpawnLocation = SpawnTransform.GetLocation();
			SpawnRotation = SpawnTransform.GetRotation().Rotator();
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation,
				SpawnRotation
			));
			if (SkillObject)
			{
#if WITH_EDITOR
				SkillObject->SetFolderPath(TEXT("SpawnedSkills/CharacterAttack"));
#endif
				SkillObject->Init(InitialSpeed, LifeTime, true);
				SkillObject->Launch(GetActorForwardVector(), Damage);
			}
			else
			{
				UE_LOG(LogPHCharacter, Error, TEXT("SkillObject is nullptr"));
			}
		}
		break;
	case EAttackType::Skill4:
		{
			APHProjectileSkillObject* SkillObject = Cast<APHProjectileSkillObject>(SpawnSkillObject(
				SkillClass,
				BaseSpawnLocation + (GetActorForwardVector() * 400.0f),
				SpawnRotation
			));
			if (SkillObject)
			{
#if WITH_EDITOR
				SkillObject->SetFolderPath(TEXT("SpawnedSkills/BossAttack/Stationary"));
#endif
				SkillObject->Launch(Damage, LifeTime);
			}
		}
		break;
	default:
		PH_LOG(LogPHCharacter, Warning, TEXT("Invalid parameter! %s"), *ToString(InAttackType));
		break;
	}
}

void APHTankerCharacter::OnPossessed()
{
	Super::OnPossessed();

	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer
		|| GetWorld()->GetNetMode() == NM_Standalone)
	{
		// 서버에서만 Overlap검사 한다.
		Weapon->OnComponentBeginOverlap.AddDynamic(this, &APHTankerCharacter::OnWeaponOverlap);
	}

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APHTankerCharacter::ServerRPCSkill1_Implementation()
{
	Super::ServerRPCSkill1_Implementation();
}

void APHTankerCharacter::ServerRPCSkill2_Implementation()
{
	Super::ServerRPCSkill2_Implementation();
}

void APHTankerCharacter::ServerRPCSkill3_Implementation()
{
	Super::ServerRPCSkill3_Implementation();
}

void APHTankerCharacter::ServerRPCSkill4_Implementation()
{
	Super::ServerRPCSkill4_Implementation();
}

void APHTankerCharacter::NormalAttack()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
			   StatDataComponent->GetSkillCooldown(EAttackType::DefaultAttack));
		return;
	}
	Super::NormalAttack();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "NormalAttack");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCNormalAttack();
}

void APHTankerCharacter::Skill1()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill1))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
			   StatDataComponent->GetSkillCooldown(EAttackType::Skill1));
		return;
	}
	Super::Skill1();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill1");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill1();
}

void APHTankerCharacter::Skill2()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill2))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
			   StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill2();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}

	ServerRPCSkill2();
}

void APHTankerCharacter::Skill3()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill3))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
			   StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill3();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill3");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}
	ServerRPCSkill3();
}

void APHTankerCharacter::Skill4()
{
	if (0.0f < StatDataComponent->GetSkillCooldown(EAttackType::Skill4))
	{
		PH_LOG(LogPHCharacter, Log, TEXT("Remaining CoolTime : %f"),
			   StatDataComponent->GetSkillCooldown(EAttackType::Skill2));
		return;
	}
	Super::Skill4();
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill4");
		FOnMontageEnded EndDelegate;
		EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
		{
			SetActionEnd();
		});
		SetMontageEndDelegate(EndDelegate);
	}
	ServerRPCSkill4();
}

void APHTankerCharacter::OnHitEnemy(const FHitResult& SweepResult)
{
	Super::OnHitEnemy(SweepResult);

	if (HitEffectClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = this;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(HitEffectClass, SweepResult.ImpactPoint,
		                                                      SweepResult.ImpactNormal.Rotation(), SpawnParams);
		SpawnedActor->SetLifeSpan(1.0f);
	}
}
