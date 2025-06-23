// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHGameInstance.h"

#include "ProjectH.h"
#include "SocketSubsystem.h"
#include "API/PHAPIClient.h"
#include "Common/Common.h"
#include "Common/GlobalEnum.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"

void UPHGameInstance::Init()
{
	Super::Init();
	// FWorldDelegates::OnWorldChanged.AddUObject(this, &UPHGameInstance::OnWorldChanged);
	bIsAttemptingConnection = false;
	bIsListenServer = false;
	bUseServer = false;
}

void UPHGameInstance::Shutdown()
{
	Super::Shutdown();
	// FWorldDelegates::OnWorldChanged.RemoveAll(this);
	if (bIsListenServer)
	{
		DeleteCurrentRoom();
	}
}

void UPHGameInstance::JoinGame()
{
	if (!bUseServer)
	{
		if (UWorld* World = GetWorld())
		{
			ENetMode NetMode = World->GetNetMode();

			if (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer)
			{
				HostServer();
			}else
			{
				TryConnectToServer(TEXT("127.0.0.1"));
			}
		}
		return;
	}
	if (!APIClient)
	{
		APIClient = NewObject<UPHAPIClient>(this);
	}
	// JSON Payload 생성
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	bool bCanBindAll;
	TSharedPtr<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(
		*GLog, bCanBindAll);
	FString MyIP = Addr->ToString(false);

	JsonObject->SetStringField(TEXT("hostIP"), MyIP);
	JsonObject->SetNumberField(TEXT("maxPlayers"), 4);

	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
	JsonWriter->Close();
	// 일단 현재 IP를 서버로 저장.
	CurrentConnectingAddress = MyIP;

	APIClient->MakeRequest<FRoomData>(API_URL_QUICK_MATCH,
	                                  ERestAPIType::POST,
	                                  [&](const FHttpResponsePtr& ResponsePtr, const FRoomData& ReceivedRoomData)
	                                  {
		                                  RoomId = ReceivedRoomData.RoomID;
		                                  UE_LOG(LogPHGameFlow, Log,
		                                         TEXT("FRoomData Received: Code=%d RoomID=%s, HostIP=%s"),
		                                         ResponsePtr.Get()->GetResponseCode(), *ReceivedRoomData.RoomID,
		                                         *ReceivedRoomData.HostIP);
		                                  if (ResponsePtr.Get()->GetResponseCode() == EHttpResponseCodes::Created)
		                                  {
			                                  HostServer();
		                                  }
		                                  else
		                                  {
			                                  TryConnectToServer(ReceivedRoomData.HostIP);
		                                  }
	                                  },
	                                  [&](const FString& ErrorMessage)
	                                  {
		                                  UE_LOG(LogPHGameFlow, Error, TEXT("FRoomData Failed: %s"), *ErrorMessage);
	                                  },
	                                  JsonString
	);
}

void UPHGameInstance::StartGame()
{
	if (!bUseServer)
	{
		return;
	}
	if (!APIClient)
	{
		APIClient = NewObject<UPHAPIClient>(this);
	}
	APIClient->MakeRequest<FRoomData>(FString::Printf(API_URL_START_GAME, *RoomId),
									  ERestAPIType::POST,
									  [&](const FHttpResponsePtr& ResponsePtr, const FRoomData& ReceivedRoomData){},
									  [&](const FString& ErrorMessage){}
	);
}

void UPHGameInstance::FinishGame()
{
	if (!bUseServer)
	{
		return;
	}
	if (bIsListenServer)
	{
		DeleteCurrentRoom();	
	}
	bIsListenServer = false;
}

void UPHGameInstance::TryConnectToServer(const FString& ServerAddress)
{
	if (bIsAttemptingConnection)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already attempting connection to another server."));
		return;
	}

	CurrentConnectingAddress = ServerAddress;
	bIsAttemptingConnection = true;

	UE_LOG(LogTemp, Log, TEXT("Attempting to connect to server: %s"), *ServerAddress);

	// @PHTODO
	// Show Connecting/Loading UI

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerAddress));

	// Timeout 처리.
	// 서버에 연결이 됐는지 확인 여부를 위함.
	GetWorld()->GetTimerManager().SetTimer(ConnectionAttemptTimerHandle, this,
	                                       &UPHGameInstance::HandleConnectionTimeout, 5.0f, false);
}

void UPHGameInstance::HostServer()
{
	// 리슨서버로 레벨 오픈
	bIsListenServer = true;
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGame"), true, "listen");
}

void UPHGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	// 새로운 월드로 성공적으로 전환되었다면, 연결 성공으로 간주
	if (bIsAttemptingConnection && NewWorld && NewWorld->IsA<UWorld>())
	{
		GetWorld()->GetTimerManager().ClearTimer(ConnectionAttemptTimerHandle);
		bIsAttemptingConnection = false;
		OnConnectionAttemptFinished.Broadcast(true);

		UE_LOG(LogTemp, Log, TEXT("Successfully connected to a new world. WorldName : %s"), *NewWorld->GetName());
		// @PHTODO
		// Hide Connecting/Loading UI
	}
}

void UPHGameInstance::HandleConnectionTimeout()
{
	// 여전히 연결 시도 중이라면 실패로 간주
	if (bIsAttemptingConnection)
	{
		bIsAttemptingConnection = false;
		OnConnectionAttemptFinished.Broadcast(false);
		UE_LOG(LogTemp, Warning, TEXT("Connection to %s timed out."), *CurrentConnectingAddress);

		// restAPI call.
		// 서버연결시도하려 했던 방 삭제 및 새로 연결.
		DeleteCurrentRoom();
		StartGame();
	}
}

void UPHGameInstance::DeleteCurrentRoom()
{
	if (!bUseServer)
	{
		return;
	}
	if (!APIClient)
	{
		APIClient = NewObject<UPHAPIClient>(this);
	}
	APIClient->MakeRequest<FRoomData>(FString::Printf(API_URL_DELETE_ROOM, *RoomId),
	                                  ERestAPIType::DELETE,
	                                  [&](const FHttpResponsePtr& ResponsePtr, const FRoomData& ReceivedRoomData)
	                                  {
										  UE_LOG(LogTemp, Display, TEXT("Delete Success! response Code : %d"),
												 ResponsePtr->GetResponseCode());
	                                  },
	                                  [&](const FString& ErrorMessage)
	                                  {
										  UE_LOG(LogPHGameFlow, Error, TEXT("Delete Failed: %s"), *ErrorMessage);
	                                  }
	);
}
