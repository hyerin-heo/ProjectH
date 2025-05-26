// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectH/Interface/Boss/PHBossPatternInterface.h"
#include "ProjectH/DataAsset/PHBossDataAsset.h"
#include "PHBossCharacterBase.generated.h"

UCLASS()
class PROJECTH_API APHBossCharacterBase : public ACharacter, public IPHBossPatternInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHBossCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void CommonPattern() override;
	virtual void SpecialPattern() override;
	virtual void SetPhase(uint8 level) override;
	

	// current phase. only can set 0~7
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Phase")
	uint8 CurrentPhaseLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Common")
	UPHBossDataAsset* DataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Common")
	TMap<uint8, FBossPhaseInfo> PhaseMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Common")
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Common")
	float HP;
	
public:	

	float GetHP() const {return HP;}
	float GetMaxHP() const {return MaxHP;}
	float GetHpPercent() const {return HP / MaxHP;}
private:
	// Phase Trigger
	EBossPhaseTriggerType TriggerType;
	float TriggerValue = 1.0f;

};
