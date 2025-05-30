// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Boss/PHBossAIInterface.h"
#include "DataAsset/PHBossDataAsset.h"
#include "PHBossCharacterBase.generated.h"

DECLARE_DELEGATE(FOnCommonAttackPattern);
USTRUCT(BlueprintType)
struct FCommonAttackPatternDelegateWrapper
{
	GENERATED_BODY()

	FCommonAttackPatternDelegateWrapper() {}
	FCommonAttackPatternDelegateWrapper(const FOnCommonAttackPattern& InDelegate)
		: ItemDelegate(InDelegate) {}

	FOnCommonAttackPattern ItemDelegate;
};
DECLARE_DELEGATE(FOnSpecialAttackPattern);
USTRUCT(BlueprintType)
struct FSpecialAttackPatternDelegateWrapper
{
	GENERATED_BODY()

	FSpecialAttackPatternDelegateWrapper() {}
	FSpecialAttackPatternDelegateWrapper(const FOnSpecialAttackPattern& InDelegate)
		: ItemDelegate(InDelegate) {}

	FOnSpecialAttackPattern ItemDelegate;
};

UCLASS()
class PROJECTH_API APHBossCharacterBase : public ACharacter, public IPHBossAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHBossCharacterBase();
	
	virtual float GetDetectionRadius() override;
	virtual float GetAttackRange() override;
	virtual float GetSpeed() override;
	virtual float GetAttackSpeed() override;
	virtual float GetArmor() override;
	virtual bool IsPhase() override;
	virtual void CommonPattern() override;
	virtual void SpecialPattern() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetPhase(uint8 level);

	virtual void SetAIPhaseDelegate(const FAIPhaseFinished& InOnPhaseFinished) override;

	FAIPhaseFinished OnPhaseFinished;
	
	UPROPERTY()
	TArray<FCommonAttackPatternDelegateWrapper> CommonAttackPatternActions;
	UPROPERTY()
	TArray<FSpecialAttackPatternDelegateWrapper> SpecialAttackPatternActions;
	

	// current phase. only can set 0~7
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	uint8 CurrentPhaseLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	UPHBossDataAsset* DataAsset;
	
	UPROPERTY()
	TMap<uint8, FBossPhaseInfo> PhaseMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Custom)
	float MaxHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Custom)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Custom)
	uint8 bIsUnableToAttack:1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float DetectionRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float Armor;
	
public:	

	float GetHP() const {return HP;}
	float GetMaxHP() const {return MaxHP;}
	float GetHpPercent() const {return HP / MaxHP;}
private:
	// Phase Trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Custom, meta=(allowPrivateAccess="true"))
	EBossPhaseTriggerType TriggerType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Custom, meta=(allowPrivateAccess="true"))
	float TriggerValue = 1.0f;

	UPROPERTY()
	FTimerHandle PhaseTimerHandle;

};
