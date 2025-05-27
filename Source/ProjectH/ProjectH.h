// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


// 네트워크 로컬 역할 정보.
#define LOG_LOCAL_ROLL_INFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))

// 네트워크 리모트 역할 정보.
#define LOG_REMOTE_ROLL_INFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))

#define LOG_SUB_LOCAL_ROLL_INFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))

#define LOG_SUB_REMOTE_ROLL_INFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))

// 넷모드(네트워크 모드) 정보.
#define LOG_NET_MODE_INFO ( (GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), (int32)GPlayInEditorID) : ( GetNetMode() == ENetMode::NM_Standalone ) ? TEXT("STANDALONE") : TEXT("SERVER"))

// 함수 호출 정보.
#define LOG_CALL_INFO ANSI_TO_TCHAR(__FUNCTION__)

// 로그 매크로.
#define PH_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NET_MODE_INFO, LOG_LOCAL_ROLL_INFO, LOG_REMOTE_ROLL_INFO, LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))

#define PH_SUB_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NET_MODE_INFO, LOG_SUB_LOCAL_ROLL_INFO, LOG_SUB_REMOTE_ROLL_INFO, LOG_CALL_INFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogPHNetwork, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPHBoss, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPHCharacter, Log, All);

