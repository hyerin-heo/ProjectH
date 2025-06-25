// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHWarriorCharacter.generated.h"

#define MAX_TURNINGTIME 6.0f;
#define MAX_BERSERKTIME 10.0f;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHWarriorCharacter : public APHCharacterBase
{
	GENERATED_BODY()

public:
	APHWarriorCharacter(const FObjectInitializer& ObjectInitializer);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void EnableSkill1Effect(bool bActive);
	void EnableSkill2Effect(bool bActive);
	void SpawnSkill3Object();


protected:
	UFUNCTION(NetMulticast, UnReliable)
	void Multicast_SetActiveSkill4Effect(bool bEffectActive);

	virtual void OnPossessed() override;
	virtual void BeginPlay() override;

	
protected:
	//Server
	//virtual void ServerRPCSkill1() override;
	virtual void ServerRPCSkill1_Implementation() override;
	virtual void ServerRPCSkill2_Implementation() override;
	virtual void ServerRPCSkill3_Implementation() override;
	virtual void ServerRPCSkill4_Implementation() override;
	
	//Attack.
	virtual void NormalAttackUI() override;
	virtual void NormalAttack() override;

	//Skill
	virtual void Skill1UI() override;
	virtual void Skill1() override;
	virtual void Skill2UI() override;
	virtual void Skill2() override;
	virtual void Skill3() override;
	virtual void Skill4() override;

	virtual void SetDead() override;
	
private:
	void StartDash();
	void HandleDashStep();

	void StartLoopTornadoSkill();
	void EndLoopTornadoSkill();

	void EndBerserkSkill();

private:
	FTimerHandle DashTimerHandle;
	FVector DashStart;
	FVector DashEnd;
	float DashDuration = 0.35f;
	float DashElapsed = 0.0f;
	float DashInterval = 0.01f;

	//TornadoSkillState;
	UPROPERTY()
	bool bTornadoSkill = false;
	UPROPERTY()
	float TornadoTurningTime = MAX_TURNINGTIME;

	//BerserkSkillState;
	UPROPERTY()
	float BerserkSkillRemaining = MAX_BERSERKTIME;
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class APHWarriorSkill3Object> SkillObjClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Skill1Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> Skill2Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Skill4Effect;
};