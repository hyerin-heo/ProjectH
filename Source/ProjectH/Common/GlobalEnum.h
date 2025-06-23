#pragma once

#include "CoreMinimal.h"

#include "GlobalEnum.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	DefaultAttack,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
	End
};

inline FString ToString(EAttackType e)
{
	switch (e)
	{
	case EAttackType::DefaultAttack:
		return TEXT("NormalAttack");
	case EAttackType::Skill1:
		return TEXT("Skill1");
	case EAttackType::Skill2:
		return TEXT("Skill2");
	case EAttackType::Skill3:
		return TEXT("Skill3");
	case EAttackType::Skill4:
		return TEXT("Skill4");
	default:
		return TEXT("UNKNOWN");
	}
}

UENUM(BlueprintType)
enum class EAttackStatType : uint8
{
	None,
	Damage,
	Heal,
	Armor
};

UENUM(BlueprintType)
enum class EClassType : uint8
{
	None,
    Warrior,
    Mage,
    Healer,
    Tanker,
	End
};

#pragma region API
 

enum class ERestAPIType
{
	GET,
	POST,
	DELETE
};

inline FString ToString(ERestAPIType e)
{
	switch (e)
	{
	case ERestAPIType::GET: return TEXT("GET");
	case ERestAPIType::POST: return TEXT("POST");
	case ERestAPIType::DELETE: return TEXT("DELETE");
	default: return TEXT("UNKNOWN");
	}
}


#pragma endregion
