// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAsset/PHCharacterStatDataAsset.h"
#include "PHCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float/*Max*/, float/*Current*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCooldownChangedDelegate, EAttackType /*SkillType*/, float /*RemainingTime*/);

USTRUCT(BlueprintType)
struct FSkillCooldownData
{
	GENERATED_BODY()

	// 스킬 타입 열거형
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cooldown")
	EAttackType SkillType = EAttackType::None;

	// 남은 쿨타임(초)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cooldown")
	float RemainingTime = 0.f;

	FSkillCooldownData() {}
	FSkillCooldownData(EAttackType InType, float InTime)
		: SkillType(InType), RemainingTime(InTime) {}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTH_API UPHCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPHCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void ReadyForReplication() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnRep_CurrentHp();

	UFUNCTION()
	void OnRep_MaxHp();

	void SetHp(float NewHp);

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	//쿨타임 변경(시작 혹은 감소) 시 알림
	FOnCooldownChangedDelegate OnCooldownChanged;
	
	FORCEINLINE const UPHCharacterStatDataAsset& GetBaseStat() const { return *StatData; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, StatData->MaxHp); OnHpChanged.Broadcast(CurrentHp, MaxHp); }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	FORCEINLINE bool GetCooldownReduction() const {return bCooldownReduction; }
	float ApplyDamage(float InDamage);

	void ResetStat();
	void StartSkillCooldown(EAttackType InAttackType);
	float GetSkillCooldown(EAttackType InAttackType);
	float GetDamage(EAttackType InAttackType);
	void IsCooldownReduction(bool IsReduction);
	void SetCooldownReductionPercentage(float InCooldownReductionPercentage);

protected:
	UPROPERTY(ReplicatedUsing= OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category= Stat)
	float CurrentHp;

	UPROPERTY(ReplicatedUsing= OnRep_MaxHp, Transient, VisibleInstanceOnly, Category= Stat)
	float MaxHp;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
	
	UPROPERTY(Replicated, Transient, VisibleInstanceOnly, Category= Stat)
	float CooldownReductionPercentage = 0.0f;

	UPROPERTY(Replicated)
	uint8 bCooldownReduction : 1;

	UPROPERTY(Replicated)
	TArray<FSkillCooldownData>RemainingCooldowns;

	//Transient를 사용하면 블루프린트에 저장이 되지 않는다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPHCharacterStatDataAsset> StatData;
		
};
