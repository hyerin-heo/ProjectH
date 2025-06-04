// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../CharacterData/Base/PHCharacterStat.h"
#include "PHCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float, float); //Max, Current
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FPHCharacterStat& /*BaseStat*/, const FPHCharacterStat& /*ModifierStat*/);


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

	UFUNCTION()
	void OnRep_CurrentHp();

	UFUNCTION()
	void OnRep_MaxHp();

	UFUNCTION()
	void OnRep_BaseStat();

	UFUNCTION()
	void OnRep_ModifierStat();

	void SetHp(float NewHp);

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;
	
	FORCEINLINE void AddBaseStat(const FPHCharacterStat& InAddBaseStat) { BaseStat = BaseStat + InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE const FPHCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FPHCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FPHCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp, MaxHp); }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage);
	void SetNewMaxHp(const FPHCharacterStat& InBaseStat, const FPHCharacterStat& InModifierStat);

	void ResetStat();

protected:
	UPROPERTY(ReplicatedUsing= OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category= Stat)
	float CurrentHp;

	UPROPERTY(ReplicatedUsing= OnRep_MaxHp, Transient, VisibleInstanceOnly, Category= Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
	
	UPROPERTY(Transient, ReplicatedUsing=OnRep_BaseStat, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPHCharacterStat BaseStat;
	
	UPROPERTY(Transient, ReplicatedUsing=OnRep_ModifierStat, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FPHCharacterStat ModifierStat;
		
};
