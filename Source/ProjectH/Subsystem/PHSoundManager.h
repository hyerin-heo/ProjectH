// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PHSoundManager.generated.h"


UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	BGM,
	SFX,
	UI,
};


/**
 * 
 */
UCLASS()
class PROJECTH_API UPHSoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 공통 로딩
	TSoftObjectPtr<USoundBase> LoadSoundByName(ESoundCategory Type, const FString& SoundName);
	//UAudioComponent* CreateAudioComponent(TSoftObjectPtr<USoundBase> SoundAsset, bool bAutoActivate);
	
	// 간단한 재생 함수
	UFUNCTION(BlueprintCallable, Category="Sound")
	void PlayBGM(const ESoundCategory Type, const FString& SoundName, float FadeInTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Sound")
	void StopBGM(float FadeOutTime = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Sound")
	void PlaySFX(const ESoundCategory Type, const FString& SoundName, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Sound")
	void PlaySFX3D(const ESoundCategory Type, const FString& SoundName, FVector Location, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Sound")
	UAudioComponent* PlayLoopingSFX(const ESoundCategory Type, const FString& SoundName, float Volume = 1.0f);

	UFUNCTION(BlueprintCallable, Category="Sound")
	void StopLoopingSFX(const FString& SoundName);

private:
	UPROPERTY()
	UAudioComponent* BGMComponent;

	//내부 캐싱
	TMap<FString, TSoftObjectPtr<USoundBase>> CachedBGMSounds;
	TMap<FString, TSoftObjectPtr<USoundBase>> CachedSFXSounds;
	TMap<FString, TSoftObjectPtr<USoundBase>> CachedUISounds;
	TMap<FString, UAudioComponent*> ActiveLoopingSFXMap;

	//경로 prefix
	const FString BaseBGMPath = TEXT("/Game/ProjectH/Sound/");
	const FString BaseSFXPath = TEXT("/Game/ProjectH/Sound/SFX/");
	const FString BaseUIPath = TEXT("/Game/ProjectH/Sound/UI/");
	
};
