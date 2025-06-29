// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PHTankerCharacter.h"

#include "ProjectH.h"
#include "Common/Common.h"
#include "Common/HitObject/PHHitEffectActor.h"
#include "Common/SkillObject/PHProjectileSkillObject.h"
#include "Component/PHCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Physics/PHCollision.h"

APHTankerCharacter::APHTankerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	Shield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield"));
	Shield->SetCollisionProfileName(CPROFILE_TRIGGER);
	Shield->SetupAttachment(GetMesh(), TEXT("LeftWeaponShield"));
	Weapon->SetIsReplicated(true);
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	
	ShieldNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldNiagaraFX"));
	ShieldNiagaraComponent->SetupAttachment(Shield);
	ShieldColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldCollider"));
	ShieldColliderComponent->SetupAttachment(ShieldNiagaraComponent);

	ShieldColliderComponent->SetCollisionProfileName(CPROFILE_SHIELD);

	// 자동 재생 비활성화
	ShieldNiagaraComponent->bAutoActivate = false;
}

void APHTankerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ShieldColliderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APHTankerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APHTankerCharacter::SetSkill(EAttackType InAttackType)
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
		
		ShieldColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APHTankerCharacter::OnShieldOverlap);
	}

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APHTankerCharacter::OnShieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}
	if (!OtherActor)
	{
		return;
	}
	if (OtherActor->ActorHasTag(TAG_ALLY) || (OtherActor->Owner && OtherActor->Owner->ActorHasTag(TAG_ALLY)))
	{
		//같은편이 쏜거임.
		return;
	}
	// @PHTODO 방향 계산하여 쉴드 방향으로 맞았는지 쉴드 뒷방향으로 맞았는지 확인 필요.
	ASkillObjectBase* SkillObject = Cast<ASkillObjectBase>(OtherActor);
	if (SkillObject)
	{
		SkillObject->ResetProjectile();
	}
}

void APHTankerCharacter::ServerRPCSkill1_Implementation()
{
	Super::ServerRPCSkill1_Implementation();
}

void APHTankerCharacter::ServerRPCSkill2_Implementation()
{
	// Super::ServerRPCSkill2_Implementation();

	bIsShieldActive = true;
	
	AttackDamage = StatDataComponent->GetDamage(EAttackType::Skill2);
	StatDataComponent->StartSkillCooldown(EAttackType::Skill2);

	ShieldNiagaraComponent->Activate(true);
	ShieldColliderComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
	GetWorldTimerManager().SetTimer(ShieldTimerHandle, [&]()
	{
		bIsShieldActive = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.2f);
		}

		ShieldNiagaraComponent->DeactivateImmediate();
		ShieldColliderComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActionEnd();
	}, 3.0f, false);

	
	Skill2RPC();
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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (!HasAuthority())
	{
		PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		
		ShieldNiagaraComponent->Activate(true);
		GetWorldTimerManager().SetTimer(ShieldTimerHandle, [&]()
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Stop(0.2f);
			}
			ShieldNiagaraComponent->DeactivateImmediate();
			SetActionEnd();
		}, 3.0f, false);
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

void APHTankerCharacter::Skill2HitRPC_Implementation()
{
	PlayAnimMontage(ActionMontage, 1.0f, "Skill2Hit");
	FOnMontageEnded EndDelegate;
	EndDelegate.BindLambda([&](UAnimMontage* Montage, bool bInterrupted)
	{
		// 아직 스킬 2중임.
		if (GetWorldTimerManager().IsTimerActive(ShieldTimerHandle))
		{
			PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		}
	});
	SetMontageEndDelegate(EndDelegate);
}

void APHTankerCharacter::Skill2RPC_Implementation()
{
	if (HasAuthority())
	{
		return;
	}
	PlayAnimMontage(ActionMontage, 1.0f, "Skill2");
		
	ShieldNiagaraComponent->Activate(true);
	GetWorldTimerManager().SetTimer(ShieldTimerHandle, [&]()
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(0.2f);
		}
		ShieldNiagaraComponent->DeactivateImmediate();
		SetActionEnd();
	}, 3.0f, false);
}

float APHTankerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsShieldActive && DamageCauser)
	{
		FVector ShieldForwardVector = ShieldColliderComponent->GetForwardVector(); 
        
		FVector DamageOrigin = DamageCauser->GetActorForwardVector();

		float DotProduct = FVector::DotProduct(ShieldForwardVector, DamageOrigin);

		if (DotProduct < KINDA_SMALL_NUMBER)
		{
			// 피해가 방패의 앞쪽에서 오는 경우
			Skill2HitRPC();
			//피해를 안입음.
			return 0.0f; 
		}
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
