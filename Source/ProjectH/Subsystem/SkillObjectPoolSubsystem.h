// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SkillObjectBase.h"
#include "Subsystems/WorldSubsystem.h"
#include "SkillObjectPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTH_API USkillObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	// 소멸될 때 호출
	virtual void Deinitialize() override;

	/**
	 * GetWorld()->GetSubsystem<UProjectilePoolSubsystem>()
	 */
	UFUNCTION(BlueprintPure, Category = Pooling, meta = (WorldContext = "WorldContextObject"))
	static USkillObjectPoolSubsystem* Get(const UObject* WorldContextObject);


protected:

	UPROPERTY(EditDefaultsOnly, Category = Pooling)
	TArray<FSkillObjectPoolData> SkillObjectPoolDataset;
	
	TMap<TSubclassOf<ASkillObjectBase>, FSkillObjectPoolData> SkillObjectPoolMap;

public:
	/**
	 * Must be called Server
	 * @param SkillObjectClass - 풀에서 가져올 투사체의 클래스 타입
	 * @return AProjectileBase 인스턴스, 또는 사용 가능한 투사체가 없으면 nullptr
	 */
	ASkillObjectBase* GetSkillObject(const TSubclassOf<ASkillObjectBase>& SkillObjectClass);
	
	ASkillObjectBase* SpawnSkillObject(const TSubclassOf<ASkillObjectBase>& SkillObjectClass, const FVector& SpawnLocation, const FRotator& SpawnRotation, const AActor* InstigatorActor, AActor* OwnerActor);

#pragma region PHTODO 추후 Pawn주체 코드로 뺄 것.
	/**
	 * Must be called Server
	 * Vector를 Start-End까지 가도록 만들 때 사용.
	 * Speed와 LifeTime둘중 하나의 값만 채울 것.
	 * 둘다 채워져 있다면 Speed를 기준으로 산출
	 * @param ProjectileClass - 풀에서 가져올 투사체의 클래스 타입
	 * @param SpawnLocation - 투사체가 스폰될 월드 위치
	 * @param InstigatorActor - 투사체를 발사한 액터의 Instigator
	 * @param OwnerActor - 투사체의 Owner 액터
	 * @param Damage - 투사체의 데미지
	 * @param EndLocation - 투사체가 도달할 위치
	 * @param Speed - 투사체 속도
	 * @param LifeTime - 투사체의 생명 주기
	 * @param ReturnToPoolOnHit - 투사체 Hit시 Pool에 반환 여부
	 * @return 활성화된 AProjectileBase 인스턴스, 또는 사용 가능한 투사체가 없으면 nullptr
	 */
	ASkillObjectBase* SpawnAndFireProjectile(const TSubclassOf<ASkillObjectBase>& ProjectileClass, const FVector& SpawnLocation, const AActor* InstigatorActor, AActor* OwnerActor, float Damage, const FVector& EndLocation, float Speed = -1.f, float LifeTime = -1.f, bool ReturnToPoolOnHit = false);
	
	/**
	 * Must be called Server
	 * Vector방향으로 LifeTime초 동안 가는 투사체 생성 
	 * @param ProjectileClass - 풀에서 가져올 투사체의 클래스 타입
	 * @param SpawnLocation - 투사체가 스폰될 월드 위치
	 * @param SpawnRotation - 투사체가 스폰될 월드 회전
	 * @param InstigatorActor - 투사체를 발사한 액터의 Instigator
	 * @param OwnerActor - 투사체의 Owner 액터
	 * @param Damage - 투사체의 데미지
	 * @param Speed - 투사체의 속도
	 * @param LifeTime - 투사체의 생명 주기
	 * @param ReturnToPoolOnHit - 투사체 Hit시 Pool에 반환 여부
	 * @return 활성화된 AProjectileBase 인스턴스, 또는 사용 가능한 투사체가 없으면 nullptr
	 */
	ASkillObjectBase* SpawnAndFireProjectile(const TSubclassOf<ASkillObjectBase>& ProjectileClass, const FVector& SpawnLocation, const FRotator& SpawnRotation, const AActor* InstigatorActor, AActor* OwnerActor, float Damage, float Speed, float LifeTime, bool ReturnToPoolOnHit = false);
	
	/**
	 * Must be called Server
	 * Vector 위치에만 생성할 때 사용.
	 * @param ProjectileClass - 풀에서 가져올 투사체의 클래스 타입
	 * @param SpawnLocation - 투사체가 스폰될 월드 위치
	 * @param SpawnRotation - 투사체가 스폰될 월드 회전
	 * @param InstigatorActor - 투사체를 발사한 액터의 Instigator
	 * @param OwnerActor - 투사체의 Owner 액터
	 * @param Damage - 투사체의 데미지
	 * @param LifeTime - 투사체의 생명 주기
	 * @return 활성화된 AProjectileBase 인스턴스, 또는 사용 가능한 투사체가 없으면 nullptr
	 */
	ASkillObjectBase* SpawnAndFireProjectile(const TSubclassOf<ASkillObjectBase>& ProjectileClass, const FVector& SpawnLocation, const FRotator& SpawnRotation, const AActor* InstigatorActor, AActor* OwnerActor, float Damage, float LifeTime);	
#pragma endregion
	
private:
	/**
	 * Must be called Server
	 */
	void InitializePools();

	/**
	 * Must be called Server
	 * @param PoolData - 초기화할 풀 설정
	 */
	void InitializeSinglePool(FSkillObjectPoolData& PoolData);
	
};
