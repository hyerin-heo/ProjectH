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
