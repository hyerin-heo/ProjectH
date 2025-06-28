// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "Interface/Character/PHCharacterShieldInterface.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "PHTankerCharacter.generated.h"

class APHHitEffectActor;
enum class EAttackType : uint8;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHTankerCharacter : public APHCharacterBase, public IPHCharacterShieldInterface
{
	GENERATED_BODY()

public:
	APHTankerCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSkill(EAttackType InAttackType);

	virtual void OnPossessed() override;

	virtual bool IsShieldActive() const override {return bIsShieldActive;}
	
	UFUNCTION()
	void OnShieldOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill|Effects", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APHHitEffectActor> HitEffectClass;

	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TMap<EAttackType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Shield;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Effects", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> ShieldColliderComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Effects", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> ShieldNiagaraComponent;

	UPROPERTY()
	uint8 bIsShieldActive:1;

	FTimerHandle ShieldTimerHandle;
};
