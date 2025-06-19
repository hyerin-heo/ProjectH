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
