// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHHealerCharacter.generated.h"


#define MAX_INVINCIBILITYTIME 4.0f;
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
	UFUNCTION(Server, Unreliable)
	void ServerRPCAllHealTarget();
	

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SetSkill3ObjectActive(bool bActive);
	
	
	void SpawnNormalAttackObject();

private:
	virtual void OnPossessed() override;
	virtual void BeginPlay() override;

protected:
	virtual void ServerRPCSkill3_Implementation() override;
	
	//Attack.
	virtual void NormalAttackUI() override;
	virtual void NormalAttack() override;
	virtual void Skill1UI() override;
	virtual void Skill1() override;
	virtual void Skill2UI() override;
	virtual void Skill2() override;
	virtual void Skill3UI() override;
	virtual void Skill3() override;
	virtual void Skill4UI() override;
	virtual void Skill4() override;

private:
	void StartLoopInvincibilitySkill();
	void EndLoopInvincibilitySkill();

private:
	UPROPERTY()
	float InvincibilityRemainingTime = MAX_INVINCIBILITYTIME;
	UPROPERTY()
	bool bInvincibilitySkill = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Object", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASkillObjectBase> NormalAttackObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APHHealerSkill3Object> Skill3ObjectClass;

	TObjectPtr<class APHHealerSkill3Object> Skill3Object;
	
	APHCharacterBase* TargetCharacter = nullptr;
	
};
