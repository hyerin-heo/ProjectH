// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/PHCharacterStatComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPHCharacterStatComponent::UPHCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;
	
	AttackRadius = 50.0f;
	
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UPHCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ResetStat();
	
	SetIsReplicated(true);

}

// Called when the game starts
void UPHCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void UPHCharacterStatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 오직 서버 권한일 때만 쿨타임 감소 처리
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return;
	}

	
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
	DOREPLIFETIME_CONDITION(UPHCharacterStatComponent, StatData, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UPHCharacterStatComponent, RemainingCooldowns, COND_OwnerOnly);
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

	ResetStat();
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


void UPHCharacterStatComponent::ResetStat()
{
	if (StatData)
	{
		MaxHp = StatData->MaxHp;
		AttackRadius = StatData->AttackRange;
		SetHp(MaxHp);

		for (const auto& KeyValue : StatData->AttackStatMap)
		{
			RemainingCooldowns.Add(FSkillCooldownData(KeyValue.Key, 0));
		}
	}
}

void UPHCharacterStatComponent::StartSkillCooldown()
{
}

float UPHCharacterStatComponent::GetSkillCooldown(EAttackType InAttackType)
{
	if (!StatData) return 1.0f;
	
	for (auto& RemainingCooldown : RemainingCooldowns)
	{
		if (InAttackType == RemainingCooldown.SkillType)
		{
			return RemainingCooldown.RemainingTime;
		}
		
		// if (InAttackType == RemainingCooldown.SkillType)
		// {
		// 	if (StatData->AttackStatMap.Contains(InAttackType))
		// 	{
		// 		RemainingCooldown.RemainingTime = StatData->AttackStatMap[InAttackType].CoolTime;
		// 	}
		// }
	}
	return 0;
}


