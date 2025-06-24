// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/SkillObject/SkillObjectBase.h"
#include "PHProjectileSkillObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHProjectileSkillObject : public ASkillObjectBase
{
	GENERATED_BODY()
public:
	APHProjectileSkillObject();

	virtual void BeginPlay() override;
	/**
	 * Must call after Init
	 * @param Direction 월드 내에서 이동할 거리와 방향(cm/s).
	 * @param InDamage 적용할 데미지
	 */
	virtual void Launch(const FVector& Direction, float InDamage);
	
	/**
	 * Must use Static Projectile
	 * @param InDamage 적용할 데미지
	 * @param InLifeTime 생명 주기
	 */
	virtual void Launch(float InDamage, float InLifeTime);

	// projectile with Speed or LifeTime.
	void Init(float InSpeed, float InLifeTime, bool ReturnToPoolOnHit);
	// projectile go to EndLocation with Speed or LifeTime.
	void Init(const FVector& InEndLocation, float InSpeed, float InLifeTime, bool ReturnToPoolOnHit);

	virtual void ResetProjectile() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Client_ActivateSkillObject_Implementation(FVector InLocation, FRotator InRotation, FVector InVelocity, float InDamage, float InLifeTime, bool bInReturnToPoolOnHit) override;

	virtual void Client_ResetProjectile_Implementation() override;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = SkillObject)
	UProjectileMovementComponent* MovementComponent;
	
protected:
	// only spawn.
	void Init(float InLifeTime);

	
};
