// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectH.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SkillObjectBase.generated.h"

class APHHitEffectActor;
class IHitEffectInterface;

UENUM(BlueprintType)
enum class ESkillObjectHitType : uint8
{
	// 기본 충돌: 한 번의 데미지를 주고 물리 임펄스 적용.
	NormalHit,
	// 틱 데미지 처리: 일정 시간 동안 주기적으로 데미지 적용.
	TickDamage,
	// 월드 고정 오브젝트(World Static) 충돌 시 재활용. 데미지는 기본처리.
	ResetOnWorldStaticHit
};

USTRUCT(BlueprintType)
struct FSkillObjectPoolData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pooling)
	TSubclassOf<ASkillObjectBase> SkillObjectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pooling, meta = (ClampMin = "1"))
	int32 PoolSize = 50;

	UPROPERTY(Transient)
	TArray<ASkillObjectBase*> PooledSkillObjects;

	UPROPERTY(Transient)
	int32 CurrentPoolIndex = 0;

	// FProjectilePoolData() : ProjectileClass(nullptr), PoolSize(50), CurrentPoolIndex(0) {}
};

UCLASS()
class PROJECTH_API ASkillObjectBase : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASkillObjectBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
						 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void ResetProjectile();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void Client_ActivateSkillObject(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit);

	UFUNCTION(NetMulticast, Reliable)
	virtual void Client_ResetProjectile();
	
	// Sphere collision component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillObject)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillObject)
	ESkillObjectHitType CurrentHitType;

protected:

	virtual void HitOnWorld(const FHitResult& HitResult);
	virtual void HitOnOpponent(const FHitResult& HitResult);
	
	UPROPERTY()
	uint8 bReturnToPoolOnHit:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillObject)
	float Damage;

	float LifeSpan;
	
	float LifeSpanDeltaTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkillObject|Effects")
	TSubclassOf<APHHitEffectActor> HitEffectComponentClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkillObject|Effects")
	TSubclassOf<APHHitEffectActor> HitWorldEffectComponentClass;

	// 틱 데미지 간격 (초)
	UPROPERTY(EditDefaultsOnly, Category = "SkillObject|Tick", meta = (EditCondition = "CurrentHitType == ESkillObjectHitType::TickDamage"))
	float TickDamageInterval;

	UPROPERTY(EditDefaultsOnly, Category = "SkillObject|Tick", meta = (EditCondition = "CurrentHitType == ESkillObjectHitType::TickDamage"))
	float TickDamageDuration; 

private:
	UPROPERTY()
	TMap<AActor*, FTimerHandle> ActiveTickDamageTargets;

	void DealTickDamage(AActor* TargetActor, float TickDamageAmount);
};
