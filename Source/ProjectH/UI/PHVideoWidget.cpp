// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHVideoWidget.h"

#include "Components/Image.h"
#include "Runtime/MediaAssets/Public/FileMediaSource.h"
#include "Runtime/MediaAssets/Public/MediaPlayer.h"
#include "Runtime/MediaAssets/Public/MediaTexture.h"

void UPHVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// 비디오를 표시할 UImage 위젯에 미디어 텍스처 연결
	if (VideoDisplayImage)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(MediaTexture);
		VideoDisplayImage->SetBrush(Brush);
		UE_LOG(LogTemp, Log, TEXT("MediaTexture assigned to VideoDisplayImage."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VideoDisplayImage is null or not bound in UMyVideoWidget."));
	}
	MediaPlayer->OnEndReached.AddDynamic(this, &UPHVideoWidget::OnFinishVideo);
}

void UPHVideoWidget::NativeDestruct()
{
	Super::NativeDestruct();
	if (MediaPlayer)
	{
		MediaPlayer->OnEndReached.RemoveDynamic(this, &UPHVideoWidget::OnFinishVideo); // 델리게이트 해제
		MediaPlayer->Close(); // 미디어 플레이어 닫기
		MediaPlayer = nullptr;
		UE_LOG(LogTemp, Log, TEXT("MediaPlayer closed and released."));
	}
	if (MediaTexture)
	{
		MediaTexture = nullptr; // UPROPERTY로 인해 자동으로 가비지 컬렉션될 것임
	}
}

void UPHVideoWidget::SetMediaSource(UFileMediaSource* InMediaSource)
{
	VideoFileSource = InMediaSource;
	// 비디오 파일 열기 및 재생
	if (VideoFileSource)
	{
		if (MediaPlayer)
		{
			MediaPlayer->OpenSource(VideoFileSource);
			UE_LOG(LogTemp, Log, TEXT("Opening media source: %s"), *VideoFileSource->GetFilePath());	
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("MediaPlayer is NULL!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("VideoFileSource not assigned in UMyVideoWidget."));
	}
}

void UPHVideoWidget::OnFinishVideo()
{
	OnFinishCinematic.Broadcast();
}
