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

enum class ERestApiUrl
{
	CreateRoom,//POST - HostIp, MaxPlayer
	QuickMatch,
	SearchRoom,//GET
	JoinRoom,//POST
	DeleteRoom//DELETE
};

inline FString ToString(ERestApiUrl e)
{
	switch (e)
	{
		case ERestApiUrl::CreateRoom: return TEXT("rooms");
		case ERestApiUrl::QuickMatch: return TEXT("rooms/quick-match");
		case ERestApiUrl::SearchRoom: return TEXT("rooms");
		case ERestApiUrl::JoinRoom: return TEXT("rooms/%s/join");//FString::Printf(*ToString(ERestApiUrl::JoinRoom), *RoomId);
		case ERestApiUrl::DeleteRoom: return TEXT("rooms/%s");//FString::Printf(*ToString(ERestApiUrl::DeleteRoom), *RoomId);
		default:
			return TEXT("unknown");
	}
}


#pragma endregion
