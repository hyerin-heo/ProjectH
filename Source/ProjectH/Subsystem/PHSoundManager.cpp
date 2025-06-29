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
		BGMComponent->FadeIn(FadeInTime, 0.5f);
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

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float Cooldown = 0.1f; // 동일 사운드 중복 재생 제한 시간 (초)
	const float MaxKeepDuration = 2.0f; // 오래된 기록 보존 시간 (초)

	// 오래된 항목 정리 (최대 1~2초 이상 지난 것 제거)
	for (auto It = RecentPlayedSFXMap.CreateIterator(); It; ++It)
	{
		if (CurrentTime - It.Value() > MaxKeepDuration)
		{
			It.RemoveCurrent();
		}
	}

	// 최근에 재생된 적 있다면 시간 차 확인
	if (RecentPlayedSFXMap.Contains(SoundName))
	{
		const float LastPlayedTime = RecentPlayedSFXMap[SoundName];
		if (CurrentTime - LastPlayedTime < Cooldown)
		{
			// 너무 짧은 시간 내 중복 → 무시
			return;
		}
	}
	
	auto SoundAsset = LoadSoundByName(Type, SoundName);
	if (SoundAsset.IsValid())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundAsset.Get(), Volume);
		RecentPlayedSFXMap.Add(SoundName, CurrentTime);
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

UAudioComponent* UPHSoundManager::PlayLoopingSFX(const ESoundCategory Type, const FString& SoundName, float Volume)
{
	if (ActiveLoopingSFXMap.Contains(SoundName))
	{
		UAudioComponent* ExistingComp = ActiveLoopingSFXMap[SoundName];
		if (ExistingComp && ExistingComp->IsPlaying())
		{
			return ExistingComp; // 이미 재생 중이면 무시
		}
	}

	auto SoundAsset = LoadSoundByName(Type, SoundName);
	if (!SoundAsset.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SoundManager] Looping SFX not found: %s"), *SoundName);
		return nullptr;
	}

	UAudioComponent* Comp = NewObject<UAudioComponent>(GetWorld()->GetWorldSettings());
	Comp->bAutoActivate = false;
	Comp->bAllowSpatialization = false;
	Comp->SetSound(SoundAsset.Get());
	Comp->SetVolumeMultiplier(Volume);
	Comp->RegisterComponent();
	Comp->Play();

	ActiveLoopingSFXMap.Add(SoundName, Comp);

	return Comp;
}

void UPHSoundManager::StopLoopingSFX(const FString& SoundName)
{
	if (ActiveLoopingSFXMap.Contains(SoundName))
	{
		UAudioComponent* Comp = ActiveLoopingSFXMap[SoundName];
		if (Comp)
		{
			Comp->FadeOut(0.5f, 0.0f); // 부드럽게 사라지게 (선택)
			Comp->Stop();
			Comp->DestroyComponent();
		}
		ActiveLoopingSFXMap.Remove(SoundName);
	}
}
