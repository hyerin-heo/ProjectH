// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PHSoundManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UPHSoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UPHSoundManager::Deinitialize()
{
	Super::Deinitialize();

	if (BGMComponent)
	{
		BGMComponent->Stop();
		BGMComponent->DestroyComponent();
	}
}

TSoftObjectPtr<USoundBase> UPHSoundManager::LoadSoundByName(ESoundCategory Type, const FString& SoundName)
{
	// 경로 구성: "/Game/Audio/SoundName.SoundName"
	
	FString FullPath = "";
	
	switch (Type)
	{
	case ESoundCategory::BGM:
		{
			if (CachedBGMSounds.Contains(SoundName))
			{
				return CachedBGMSounds[SoundName];
			}

			FullPath = BaseBGMPath + SoundName + TEXT(".") + SoundName;
			break;
		}
	case ESoundCategory::SFX:
		{
			if (CachedSFXSounds.Contains(SoundName))
			{
				return CachedSFXSounds[SoundName];
			}
			FullPath = BaseSFXPath + SoundName + TEXT(".") + SoundName;
			break;
		}
	case ESoundCategory::UI:
		{
			if (CachedUISounds.Contains(SoundName))
			{
				return CachedUISounds[SoundName];
			}
			FullPath = BaseSFXPath + SoundName + TEXT(".") + SoundName;
			break;
		}
	}
	
	FSoftObjectPath AssetPath(FullPath);
	TSoftObjectPtr<USoundBase> SoundAsset(AssetPath);
	switch (Type)
	{
	case ESoundCategory::BGM:
		{
			CachedBGMSounds.Add(SoundName, SoundAsset);
			break;
		}
	case ESoundCategory::SFX:
		{
			CachedSFXSounds.Add(SoundName, SoundAsset);
			break;
		}
	case ESoundCategory::UI:
		{
			CachedUISounds.Add(SoundName, SoundAsset);
			break;
		}
	}
	
	SoundAsset.LoadSynchronous();

	return SoundAsset;
}

// UAudioComponent* UPHSoundManager::CreateAudioComponent(TSoftObjectPtr<USoundBase> SoundAsset, bool bAutoActivate)
// {
// 	if (!SoundAsset.IsValid())
// 	{
// 		SoundAsset.LoadSynchronous();
// 	}
//
// 	if (SoundAsset.IsValid())
// 	{
// 		UAudioComponent* Comp = NewObject<UAudioComponent>(this);
// 		Comp->RegisterComponent();
// 		Comp->SetSound(SoundAsset.Get());
// 		Comp->bAutoActivate = bAutoActivate;
// 		return Comp;
// 	}
// 	return nullptr;
// }

void UPHSoundManager::PlayBGM(const ESoundCategory Type, const FString& SoundName, float FadeInTime)
{
	auto SoundAsset = LoadSoundByName(Type, SoundName);
	if (!SoundAsset.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[SoundManager] BGM SoundAsset invalid: %s"), *SoundName);
		return;
	}

	if (BGMComponent)
	{
		BGMComponent->FadeOut(FadeInTime, 0.0f);
		BGMComponent->Stop();
		BGMComponent->DestroyComponent();
		BGMComponent = nullptr;
	}

	// ✅ SpawnSound2D로 월드에 붙여서 생성
	BGMComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundAsset.Get(), 1.0f, 1.0f, 0.0f, nullptr, true);

	if (BGMComponent)
	{
		BGMComponent->bAllowSpatialization = false;
		BGMComponent->FadeIn(FadeInTime, 1.0f);
	}
}

void UPHSoundManager::StopBGM(float FadeOutTime)
{
	if (BGMComponent)
	{
		BGMComponent->FadeOut(FadeOutTime, 0.0f);
	}
}

void UPHSoundManager::PlaySFX(const ESoundCategory Type, const FString& SoundName, float Volume)
{
	auto SoundAsset = LoadSoundByName(Type, SoundName);
	if (SoundAsset.IsValid())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundAsset.Get(), Volume);
	}
}

void UPHSoundManager::PlaySFX3D(const ESoundCategory Type, const FString& SoundName, FVector Location, float Volume)
{
	auto SoundAsset = LoadSoundByName(Type, SoundName);
	if (SoundAsset.IsValid())
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundAsset.Get(), Location, Volume);
	}
}
