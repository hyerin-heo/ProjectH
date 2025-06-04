// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/PHCharacterStatComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPHCharacterStatComponent::UPHCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	CurrentLevel = 1;
	AttackRadius = 50.0f;
	
	bWantsInitializeComponent = true;
	
}

void UPHCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ResetStat();
	
	//스탯 변경 이벤트에 최대 체력 설정 함수 등록.
	OnStatChanged.AddUObject(this, &UPHCharacterStatComponent::SetNewMaxHp);

	SetIsReplicated(true);
}

// Called when the game starts
void UPHCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPHCharacterStatComponent::ReadyForReplication()
{
	Super::ReadyForReplication();
}

void UPHCharacterStatComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//HP 프로퍼티는 모든 클라이언트에게 전송.
	DOREPLIFETIME(UPHCharacterStatComponent, CurrentHp);
	DOREPLIFETIME(UPHCharacterStatComponent, MaxHp);

	//캐릭터를 소유한 클라이언트만 전송하도록 설정.
	DOREPLIFETIME_CONDITION(UPHCharacterStatComponent, BaseStat, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPHCharacterStatComponent, ModifierStat, COND_OwnerOnly);
}

void UPHCharacterStatComponent::OnRep_CurrentHp()
{
	//서버로부터 받은 변경된 CurrentHP 정보를 델리게이트를 통해 알림.
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	
	//죽었는지 확인.
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
}

void UPHCharacterStatComponent::OnRep_MaxHp()
{
	//서버로부터 받은 변경된 CurrentHP 정보를 델리게이트를 통해 알림.
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UPHCharacterStatComponent::OnRep_BaseStat()
{
	//스탯 변경 이벤트 발행.
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UPHCharacterStatComponent::OnRep_ModifierStat()
{
	//스탯 변경 이벤트 발행.
	OnStatChanged.Broadcast(BaseStat, ModifierStat);
}

void UPHCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

float UPHCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	
	SetHp(PrevHp - ActualDamage);

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void UPHCharacterStatComponent::SetNewMaxHp(const FPHCharacterStat& InBaseStat, const FPHCharacterStat& InModifierStat)
{
}

void UPHCharacterStatComponent::ResetStat()
{
	MaxHp = BaseStat.MaxHp;
	SetHp(MaxHp);
}


