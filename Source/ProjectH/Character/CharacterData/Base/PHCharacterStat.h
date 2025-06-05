#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PHCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FPHCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FPHCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Stat)
	float Attack;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	FPHCharacterStat operator+(const FPHCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FPHCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FPHCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		// 저장.
		uint32 uMaxHp = (uint32)MaxHp;
		Ar.SerializeIntPacked(uMaxHp);
		MaxHp = (float)uMaxHp;            // 불러오기.

		// 저장.
		uint32 uAttack = (uint32)Attack;
		Ar.SerializeIntPacked(uAttack);
		Attack = (float)uAttack;            // 불러오기.

		// 저장.
		uint32 uAttackRange = (uint32)AttackRange;
		Ar.SerializeIntPacked(uAttackRange);
		AttackRange = (float)uAttackRange;            // 불러오기.

		// 저장.
		uint32 uAttackSpeed = (uint32)AttackSpeed;
		Ar.SerializeIntPacked(uAttackSpeed);
		AttackSpeed = (float)uAttackSpeed;            // 불러오기.

		// 저장.
		uint32 uMovementSpeed = (uint32)MovementSpeed;
		Ar.SerializeIntPacked(uMovementSpeed);
		MovementSpeed = (float)uMovementSpeed;            // 불러오기.

		return true;
	}
	
};

template<>
struct TStructOpsTypeTraits<FPHCharacterStat> : public TStructOpsTypeTraitsBase2<FPHCharacterStat>
{
	enum my_enum
	{
		WithNetSerializer = true,
	};
};
