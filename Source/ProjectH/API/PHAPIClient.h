// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/NoExportTypes.h"
#include "PHAPIClient.generated.h"

enum class ERestAPIType;

USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString RoomID;

	UPROPERTY(BlueprintReadWrite)
	FString HostIP;

};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnRoomDataReceived, FRoomData, ReceivedRoomData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAPIRequestFailed, FString, ErrorMessage);

constexpr const TCHAR* BaseUrl = TEXT("http://192.168.20.108:3000/");
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHAPIClient : public UObject
{
	GENERATED_BODY()

public:
	template<typename TStructType>
	void MakeRequest (const FString& URL,
						const ERestAPIType& Type,
						const TFunction<void(FHttpResponsePtr ResponsePtr, const TStructType&)>& OnSuccess,
						const TFunction<void(const FString&)>& OnFailure,
						const FString& PayloadJson = TEXT(""));
};
