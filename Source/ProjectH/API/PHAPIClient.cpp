// Fill out your copyright notice in the Description page of Project Settings.


#include "API/PHAPIClient.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Common/GlobalEnum.h"
#include "Interfaces/IHttpResponse.h"

template <typename TStructType>
void UPHAPIClient::MakeRequest (const FString& URL,
								  const ERestAPIType& Type,
                                  const TFunction<void(FHttpResponsePtr ResponsePtr, const TStructType&)>& OnSuccess,
                                  const TFunction<void(const FString&)>& OnFailure,
                                  const FString& PayloadJson)
{
	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = HttpModule.CreateRequest();

	Request->SetVerb(ToString(Type));
	Request->SetURL(FString(BaseUrl).Append(URL));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	if (Type == ERestAPIType::POST)
	{
		if (!PayloadJson.IsEmpty())
		{
			Request->SetContentAsString(PayloadJson);
		}
		else
		{
			// content가 없는 경우도 있음.
			// FString ErrorMessage = FString::Printf(TEXT("%s request requires a non-empty PayloadJson."), *ToString(Type));
			// OnFailure(ErrorMessage);
			// UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
			// return;
			Request->SetHeader(TEXT("Content-Length"), TEXT("0"));
		}
	}

	TSharedRef<TStructType> DataStruct = MakeShared<TStructType>();
	TSharedPtr<void> DataStructAsVoid = DataStruct;

	// Callback
	Request->OnProcessRequestComplete().BindLambda(
		[OnSuccess, OnFailure, DataStruct](FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool bWasSuccessful)
	{
			if (!bWasSuccessful || !ResponsePtr.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("HTTP Request failed or invalid response."));
				OnFailure(TEXT("HTTP Request failed or invalid response."));
				return;
			}

			if (EHttpResponseCodes::IsOk(ResponsePtr->GetResponseCode()))
			{
				if (ResponsePtr->GetResponseCode() == EHttpResponseCodes::NoContent)
				{
					OnSuccess(ResponsePtr, TStructType()); // 성공 콜백 호출
					return;
				}
				FString ResponseContent = ResponsePtr->GetContentAsString();
				UE_LOG(LogTemp, Log, TEXT("Response received: %s"), *ResponseContent);

				TStructType StructType = DataStruct.Get();
				if (FJsonObjectConverter::JsonObjectStringToUStruct(ResponseContent, &StructType, 0, 0, false))
				{
					OnSuccess(ResponsePtr, StructType); // 성공 콜백 호출
				}
				else
				{
					// 파싱 실패
					FString ErrorMessage = FString::Printf(TEXT("Failed to parse JSON response for type %s. Response: %s"),
														   *TStructType::StaticStruct()->GetName(),
														   *ResponseContent);
					OnFailure(ErrorMessage);
					UE_LOG(LogTemp, Error, TEXT("%s"), *ErrorMessage);
				}
			}
			else
			{
				FString ErrorMsg = FString::Printf(TEXT("HTTP Error: %d - %s"), ResponsePtr->GetResponseCode(),
										   *ResponsePtr->GetContentAsString());
				UE_LOG(LogTemp, Error, TEXT("HTTP Error: %s"), *ErrorMsg);
				OnFailure(ErrorMsg);
			}
	});

	Request->ProcessRequest();

	UE_LOG(LogTemp, Log, TEXT("%s Request sent to: %s"), *ToString(Type), *URL);
}

template void UPHAPIClient::MakeRequest<FRoomData>(const FString& URL,
													const ERestAPIType& Type,
													const TFunction<void(FHttpResponsePtr ResponsePtr, const FRoomData&)>& OnSuccess,
													const TFunction<void(const FString&)>& OnFailure,
													const FString& PayloadJson);