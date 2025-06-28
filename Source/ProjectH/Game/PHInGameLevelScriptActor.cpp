// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/PHInGameLevelScriptActor.h"

#include "Kismet/GameplayStatics.h"

bool APHInGameLevelScriptActor::LoadStageLevel(int32 Index)
{
	if (StageLevelNames.Num() <= Index)
	{
		return false;
	}
	UGameplayStatics::LoadStreamLevel(this, StageLevelNames[Index], true, false, FLatentActionInfo());
	LoadStageLevelRPC(Index);
	if (Index - 1 >= 0)
	{
		UnLoadStageLevel(Index - 1);
	}
	return true;
}

void APHInGameLevelScriptActor::LoadStageLevelRPC_Implementation(int32 Index)
{
	if (!HasAuthority())
	{
		UGameplayStatics::LoadStreamLevel(this, StageLevelNames[Index], true, false, FLatentActionInfo());
	}
}

void APHInGameLevelScriptActor::UnLoadStageLevel(int32 Index)
{
	auto Level = UGameplayStatics::GetStreamingLevel(GetWorld(), StageLevelNames[Index]);
	Level->SetShouldBeVisible(false);
	UGameplayStatics::UnloadStreamLevel(this, StageLevelNames[Index], FLatentActionInfo(), false);
	UnLoadStageLevelRPC(Index);
}

void APHInGameLevelScriptActor::UnLoadStageLevelRPC_Implementation(int32 Index)
{
	if (!HasAuthority())
	{
		auto Level = UGameplayStatics::GetStreamingLevel(GetWorld(), StageLevelNames[Index]);
		Level->SetShouldBeVisible(false);
		UGameplayStatics::UnloadStreamLevel(this, StageLevelNames[Index], FLatentActionInfo(), false);
	}
}

void APHInGameLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}

void APHInGameLevelScriptActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("APHInGameLevelScriptActor PostInitializeComponents Called!"));
	LoadedStreamingLevelsCount = 0;
	int32 UniqueLatentActionID = 0;
	for (const FName& StageLevelName : StageLevelNames)
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnStreamLevelLoaded");
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = UniqueLatentActionID++; 

		UGameplayStatics::LoadStreamLevel(this, StageLevelName, false, false, LatentInfo);
	}
}

void APHInGameLevelScriptActor::OnStreamLevelLoaded()
{
	LoadedStreamingLevelsCount++;
}
