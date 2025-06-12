// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectH.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SkillObjectBase.generated.h"

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
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PostInitializeComponents() override;

	/**
	 * Must call after Init
	 * @param Direction 월드 내에서 이동할 거리와 방향(cm/s).
	 * @param InDamage 적용할 데미지
	 */
	void Launch(const FVector& Direction, float InDamage);
	
	/**
	 * Must use Static Projectile
	 * @param InDamage 적용할 데미지
	 * @param InLifeTime 생명 주기
	 */
	void Launch(float InDamage, float InLifeTime);

	// projectile with Speed or LifeTime.
	void Init(float InSpeed, float InLifeTime, bool ReturnToPoolOnHit);
	// projectile go to EndLocation with Speed or LifeTime.
	void Init(const FVector& InEndLocation, float InSpeed, float InLifeTime, bool ReturnToPoolOnHit);

	void ResetProjectile();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Client, Reliable)
	void Client_ActivateSkillObject(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit);

	UFUNCTION(Client, Reliable)
	void Client_ResetProjectile();
	
	// Sphere collision component.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillObject)
	UCapsuleComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = SkillObject)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SkillObject)
	UStaticMeshComponent* Mesh;

protected:
	// only spawn.
	void Init(float InLifeTime);
	
	UPROPERTY()
	uint8 bReturnToPoolOnHit:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float Damage;

	float LifeSpan;
	
	float LifeSpanDeltaTime;
};
