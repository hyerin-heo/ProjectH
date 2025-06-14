// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/PHUInt8.h"
#include "GameFramework/Character.h"
#include "Interface/Boss/PHBossAIInterface.h"
#include "DataAsset/PHBossDataAsset.h"
#include "Interface/Boss/PHBossAnimationAttackInterface.h"
#include "PHBossCharacterBase.generated.h"


DECLARE_DELEGATE(FOnAttackPattern);
USTRUCT(BlueprintType)
struct FAttackPatternDelegateWrapper
{
	GENERATED_BODY()

	FAttackPatternDelegateWrapper() {}
	FAttackPatternDelegateWrapper(const FOnAttackPattern& InDelegate, const FBossPatternInfo& InPatternInfo)
		: ItemDelegate(InDelegate), PatternInfo(InPatternInfo) {}

	FOnAttackPattern ItemDelegate;
	UPROPERTY()
	FBossPatternInfo PatternInfo;
};

UCLASS()
class PROJECTH_API APHBossCharacterBase : public ACharacter, public IPHBossAIInterface, public IPHBossAnimationAttackInterface
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
	virtual void AttackAction() override;
	virtual void PatternAction() override;
	virtual void PhasePatternAction() override;

	virtual void AttackHitCheck() override;

	virtual void PatternHitCheck(const int32& InPatternIndex, const uint8& InStep) override;

	// Animation/Effect RPC
	UFUNCTION(NetMulticast, Reliable)
	virtual void AttackActionRPC() override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void PatternActionRPC(const FAttackPatternDelegateWrapper& InPatternInfo, const FName InMontageName);
	UFUNCTION(NetMulticast, Reliable)
	virtual void PhasePatternActionRPC(const FAttackPatternDelegateWrapper& InPhasePatternInfo, const FName InMontageName);

	// Damage RPC
	// @PHTODO

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnRep_MaxHP();
	UFUNCTION()
	void OnRep_HP();

	virtual bool IsCoolTime() override;
	virtual void SetCoolTime();

	void AttackActionEnd(UAnimMontage* AnimMontage, bool bArg);
	void PatternAttackActionEnd(UAnimMontage* AnimMontage, bool bArg);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished) override;
	virtual void SetAIPatternAttackDelegate(const FAIPatternAttackFinished& InOnPatternAttackFinished) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FAIAttackFinished OnAttackFinished;
	FAIPatternAttackFinished OnPatternAttackFinished;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	FName DefaultActionName;
	// pattern name set PatternActionName1, PatternActionName2,....
	// Montage section name must Start index 0.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	FName PatternActionName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ActionMontage;
	
	UPROPERTY()
	TArray<FAttackPatternDelegateWrapper> AttackPatternActions;

	// current phase. only can set 0~7
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	FPHUInt8 CurrentPhaseLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Custom)
	UPHBossDataAsset* DataAsset;
	
	UPROPERTY()
	TMap<uint8, FBossPhaseInfo> PhaseMap;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_MaxHP, BlueprintReadOnly, Category=Custom)
	float MaxHP;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_HP, BlueprintReadOnly, Category=Custom)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float DetectionRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float AttackSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Custom)
	float Armor;

	virtual void PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase);
	
public:	

	float GetHP() const {return HP;}
	float GetMaxHP() const {return MaxHP;}
	float GetHpPercent() const {return HP / MaxHP;}
private:

	UPROPERTY()
	FTimerHandle PhaseTimerHandle;

	UPROPERTY()
	FTimerHandle CoolTimeTimerHandle;

	UPROPERTY()
	float CurrentPatternCoolTime;
	UPROPERTY()
	int32 CurrentPatternIndex;

	UPROPERTY()
	uint8 bIsPhaseTimerDone :1;
};
