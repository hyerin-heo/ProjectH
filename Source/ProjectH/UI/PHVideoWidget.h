// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PHVideoWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFinishCinematic)

class UFileMediaSource;
class UImage;
class UMediaTexture;
class UMediaPlayer;
/**
 * 
 */
UCLASS()
class PROJECTH_API UPHVideoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SetMediaSource(UFileMediaSource* InMediaSource);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video Playback")
	UFileMediaSource* VideoFileSource;

	UPROPERTY(meta = (BindWidget))
	UImage* VideoDisplayImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video Playback")
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video Playback")
	UMediaTexture* MediaTexture;

	FOnFinishCinematic OnFinishCinematic;

private:

	UFUNCTION()
	void OnFinishVideo();

protected:
	// 외부에서 UMediaPlayer를 참조할 수 있도록 Getter 제공
	UFUNCTION()
	UMediaPlayer* GetMediaPlayer() const { return MediaPlayer; }
};
