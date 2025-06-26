// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "PHTankerCharacter.generated.h"

class APHHitEffectActor;
enum class EAttackType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHTankerCharacter : public APHCharacterBase
{
	GENERATED_BODY()

public:
	APHTankerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSkill(EAttackType InAttackType, uint8 Step);

	virtual void OnPossessed() override;

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

	virtual void OnHitEnemy(const FHitResult& SweepResult) override;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill|Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APHHitEffectActor> HitEffectClass;

	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TMap<EAttackType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Shield;
};
