// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHMageCharacter.generated.h"

class ASkillObjectBase;
enum class EAttackType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHMageCharacter : public APHCharacterBase
{
	GENERATED_BODY()
public:
	APHMageCharacter(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSkill(EAttackType InAttackType, uint8 InStep);

protected:
	//Server
	virtual void ServerRPCSkill1_Implementation() override;
	virtual void ServerRPCSkill2_Implementation() override;
	virtual void ServerRPCSkill3_Implementation() override;
	virtual void ServerRPCSkill4_Implementation() override;
	
	//Attack.
	virtual void NormalAttack() override;

	//Skill
	virtual void Skill1() override;
	virtual void Skill2() override;
	virtual void Skill3() override;
	virtual void Skill4() override;
	
private:

	void SetTeleport();
	
	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TMap<EAttackType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;

};
