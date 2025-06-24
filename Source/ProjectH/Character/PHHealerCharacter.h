// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHHealerCharacter.generated.h"


#define MAX_INVINCIBILITYTIME = 6.0f;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHHealerCharacter : public APHCharacterBase
{
	GENERATED_BODY()

public:
	APHHealerCharacter(const FObjectInitializer& ObjectInitializer);
	
	//프로퍼티 리플리케이션
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Unreliable)
	void ServerRPCHealTarget(APHCharacterBase* InHealTargetCharacter);
	
	void SpawnNormalAttackObject();

private:
	virtual void OnPossessed() override;

protected:
	//Attack.
	virtual void NormalAttackUI() override;
	virtual void NormalAttack() override;
	virtual void Skill1UI() override;
	virtual void Skill1() override;

private:
	UPROPERTY(Replicated, meta = (AllowPrivateAccess = "true"))
	float InvincibilityRemainingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASkillObjectBase> NormalAttackObject;

	APHCharacterBase* HealTargetCharacter = nullptr;
	
};
