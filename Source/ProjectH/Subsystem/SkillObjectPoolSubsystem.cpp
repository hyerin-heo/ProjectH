// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SkillObjectPoolSubsystem.h"

bool USkillObjectPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 게임 월드에서만 생성되도록.
	UWorld* World = Cast<UWorld>(Outer);
	if (World && World->IsGameWorld() && !SkillObjectPoolDataset.IsEmpty())
	{
		return Super::ShouldCreateSubsystem(Outer);
	}
	return false;
}

void USkillObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("USkillObjectPoolSubsystem initialized. Role: %s"),
	       GetWorld()->GetNetMode() == NM_DedicatedServer ? TEXT("Dedicated Server") :
	       GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("Listen Server") : TEXT("Client"));

	// 클라이언트는 서버가 스폰한 풀을 복제하도록 
	if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer
		|| GetWorld()->GetNetMode() == NM_Standalone)
	{
		InitializePools();
	}
}

void USkillObjectPoolSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("USkillObjectPoolSubsystem deinitialized."));
	Super::Deinitialize();

	SkillObjectPoolMap.Empty();
}

USkillObjectPoolSubsystem* USkillObjectPoolSubsystem::Get(const UObject* WorldContextObject)
{
	if (!WorldContextObject || !WorldContextObject->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("USkillObjectPoolSubsystem::Get - Invalid WorldContextObject."));
		return nullptr;
	}
	return WorldContextObject->GetWorld()->GetSubsystem<USkillObjectPoolSubsystem>();
}

ASkillObjectBase* USkillObjectPoolSubsystem::GetSkillObject(const TSubclassOf<ASkillObjectBase>& SkillObjectClass)
{
	if (GetWorld()->GetNetMode() == NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSkillObject called on client. This should only be called on the server."));
		return nullptr;
	}

	FSkillObjectPoolData* PoolData = SkillObjectPoolMap.Find(SkillObjectClass);
	if (!PoolData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pool for SkillObjectClass %s not found. Check FSkillObjectPoolData."),
		       *SkillObjectClass->GetName());
		return nullptr;
	}

	if (PoolData->PooledSkillObjects.IsEmpty())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Pool for %s is empty. Consider increasing PoolSize or implementing dynamic pool expansion."),
		       *SkillObjectClass->GetName());
		return nullptr;
	}

	for (int32 i = 0; i < PoolData->PooledSkillObjects.Num(); ++i)
	{
		ASkillObjectBase* CurrentSkillObject = PoolData->PooledSkillObjects[PoolData->CurrentPoolIndex];
		PoolData->CurrentPoolIndex = (PoolData->CurrentPoolIndex + 1) % PoolData->PooledSkillObjects.Num();

		if (CurrentSkillObject && CurrentSkillObject->IsHidden() && !CurrentSkillObject->GetActorEnableCollision())
		{
			return CurrentSkillObject;
		}
	}

	// 사용 가능한 투사체가 풀에 없는 경우
	UE_LOG(LogTemp, Warning,
	       TEXT(
		       "No available SkillObject of type %s in pool. Consider increasing PoolSize or implementing dynamic pool expansion."
	       ), *SkillObjectClass->GetName());
	return nullptr;
}

#pragma region PHTODO 추후 Pawn주체 코드로 뺄 것.

ASkillObjectBase* USkillObjectPoolSubsystem::SpawnAndFireProjectile(
	const TSubclassOf<ASkillObjectBase>& ProjectileClass,
	const FVector& SpawnLocation, const AActor* InstigatorActor, AActor* OwnerActor, float Damage,
	const FVector& EndLocation, float Speed, float LifeTime, bool ReturnToPoolOnHit)
{
	FVector Direction = EndLocation - SpawnLocation;
	ASkillObjectBase* Projectile = SpawnSkillObject(ProjectileClass, SpawnLocation, Direction.Rotation(),
	                                                InstigatorActor, OwnerActor);
	if (Projectile)
	{
		Projectile->Init(EndLocation, Speed, LifeTime, ReturnToPoolOnHit);
		Projectile->Launch(Direction.GetSafeNormal(), Damage);
	}
	return Projectile;
}

ASkillObjectBase* USkillObjectPoolSubsystem::SpawnAndFireProjectile(
	const TSubclassOf<ASkillObjectBase>& ProjectileClass,
	const FVector& SpawnLocation, const FRotator& SpawnRotation, const AActor* InstigatorActor, AActor* OwnerActor,
	float Damage,
	float Speed, float LifeTime, bool ReturnToPoolOnHit)
{
	ASkillObjectBase* Projectile = SpawnSkillObject(ProjectileClass, SpawnLocation, SpawnRotation, InstigatorActor,
	                                                OwnerActor);
	if (Projectile)
	{
		Projectile->Init(Speed, LifeTime, ReturnToPoolOnHit);
		Projectile->Launch(SpawnRotation.Vector(), Damage);
	}
	return Projectile;
}

ASkillObjectBase* USkillObjectPoolSubsystem::SpawnAndFireProjectile(
	const TSubclassOf<ASkillObjectBase>& ProjectileClass,
	const FVector& SpawnLocation, const FRotator& SpawnRotation, const AActor* InstigatorActor, AActor* OwnerActor,
	float Damage,
	float LifeTime)
{
	ASkillObjectBase* Projectile = SpawnSkillObject(ProjectileClass, SpawnLocation, SpawnRotation, InstigatorActor,
	                                                OwnerActor);
	if (Projectile)
	{
		Projectile->Launch(SpawnRotation.Vector(), Damage);
	}
	return Projectile;
}

#pragma endregion

ASkillObjectBase* USkillObjectPoolSubsystem::SpawnSkillObject(const TSubclassOf<ASkillObjectBase>& SkillObjectClass,
                                                              const FVector& SpawnLocation,
                                                              const FRotator& SpawnRotation,
                                                              const AActor* InstigatorActor, AActor* OwnerActor)
{
	ASkillObjectBase* SkillObject = GetSkillObject(SkillObjectClass);
	if (SkillObject)
	{
		// 서버에서 설정하면 클라로 복제됨.
		SkillObject->SetActorLocation(SpawnLocation);
		SkillObject->SetActorRotation(SpawnRotation);
		SkillObject->SetActorHiddenInGame(false);
		SkillObject->SetActorEnableCollision(true);
		SkillObject->SetActorTickEnabled(true);

		SkillObject->SetInstigator(InstigatorActor->GetInstigator());
		SkillObject->SetOwner(OwnerActor);

		return SkillObject;
	}

	// 사용 가능한 투사체가 풀에 없는 경우
	UE_LOG(LogTemp, Warning,
	       TEXT(
		       "No available SkillObject of type %s in pool. Consider increasing PoolSize or implementing dynamic pool expansion."
	       ), *SkillObjectClass->GetName());
	return nullptr;
}

void USkillObjectPoolSubsystem::InitializePools()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("USkillObjectPoolSubsystem: Invalid World during InitializePools."));
		return;
	}

	SkillObjectPoolMap.Empty();

	for (FSkillObjectPoolData ConfigData : SkillObjectPoolDataset)
	{
		InitializeSinglePool(ConfigData);
	}

	UE_LOG(LogTemp, Log, TEXT("Initialized %d SkillObject pools based on configs."), SkillObjectPoolMap.Num());
}

void USkillObjectPoolSubsystem::InitializeSinglePool(FSkillObjectPoolData& PoolData)
{
	if (!PoolData.SkillObjectClass)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("A SkillObjectClass is not set in one of the FSkillObjectPoolData. Skipping this entry."));
		return;
	}
	SkillObjectPoolMap.Add(PoolData.SkillObjectClass, FSkillObjectPoolData());
	FSkillObjectPoolData& PoolRef = SkillObjectPoolMap.FindChecked(PoolData.SkillObjectClass);
	PoolRef.SkillObjectClass = PoolData.SkillObjectClass;
	PoolRef.PoolSize = PoolData.PoolSize;

	UWorld* World = GetWorld();
	if (!World) return;

	if (!PoolRef.SkillObjectClass) return;

	PoolRef.PooledSkillObjects.Empty();
	PoolRef.CurrentPoolIndex = 0;

	for (int32 i = 0; i < PoolRef.PoolSize; ++i)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// 스폰 실패 시에도 크래시 방지
		SpawnParams.bNoFail = true;

		FVector SpawnLocation = FVector::Zero();
		FRotator SpawnRotation = FRotator::ZeroRotator;

		// 서버에만 존재하며 클라에는 복제
		ASkillObjectBase* NewSkillObject = World->SpawnActor<ASkillObjectBase>(
			PoolRef.SkillObjectClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewSkillObject)
		{
			// 스폰 후 비활성화
			NewSkillObject->SetActorHiddenInGame(true);
			NewSkillObject->SetActorEnableCollision(false);
			NewSkillObject->SetActorTickEnabled(false);
			// 비활성 상태이므로 수명 제한 없음
			NewSkillObject->SetLifeSpan(0.0f);

			PoolRef.PooledSkillObjects.Add(NewSkillObject);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn SkillObject of class %s for pooling."),
			       *PoolRef.SkillObjectClass->GetName());
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Initialized pool for class %s with %d SkillObject."),
	       *PoolRef.SkillObjectClass->GetName(), PoolRef.PooledSkillObjects.Num());
}
