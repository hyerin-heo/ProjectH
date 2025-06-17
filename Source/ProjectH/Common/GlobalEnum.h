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
	None = 0,
	Damage,
	Heal,
	Armor
};