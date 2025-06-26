// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Base/PHCharacterBase.h"
#include "NiagaraComponent.h"
#include "PHMageCharacter.generated.h"

class UBoxComponent;
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
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSkill(EAttackType InAttackType, uint8 InStep);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void TakeTickDamage();

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
	virtual void Skill3UI() override;
	virtual void Skill3() override;
	virtual void Skill4() override;

	UFUNCTION(Server, Reliable)
	void ServerRPCCursorPosition(FVector InCursorPosition);
	
private:

	void SetTeleport();

	void StartLoopLaserSkill();

	FVector CursorPosition;
	
	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	TMap<EAttackType, TSubclassOf<ASkillObjectBase>> SkillObjectsMap;

	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* Skill1Component;

	UPROPERTY(EditAnywhere, Category = Skill, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Skill1BoxComponent;

	UPROPERTY()
	TArray<AActor*> ActiveTickDamageTargets;

	FTimerHandle TickTimerHandle;
	
	const float BaseLifetime = 2.f;
	const float BaseSpeed = 1500.0f;
};
