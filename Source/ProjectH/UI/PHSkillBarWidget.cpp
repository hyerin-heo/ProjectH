// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHSkillBarWidget.h"

#include "PHCharacterSkillIconWidget.h"
#include "ProjectH.h"

UPHSkillBarWidget::UPHSkillBarWidget(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	
	
}

void UPHSkillBarWidget::SetUpSkillIcons(EAttackType InAttackType, UTexture2D* NewTexture)
{
	if (InAttackType == EAttackType::DefaultAttack) return;
	
	if (SkillIconMap.Contains(InAttackType))
	{
		SkillIconMap[InAttackType]->InitializeSkillIcon(InAttackType, NewTexture);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("There is no skill icon UI object."));
}

void UPHSkillBarWidget::UpdateCooldownTime(EAttackType InAttackType, float CurrentCoolTime, float MaxCoolTime)
{
	if (InAttackType == EAttackType::DefaultAttack) return;
	
	if (SkillIconMap.Contains(InAttackType))
	{
		SkillIconMap[InAttackType]->SetCoolTimeProgress(CurrentCoolTime, MaxCoolTime);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("There is no skill icon UI object."));
}

void UPHSkillBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (int8 Type = (int8)EAttackType::Skill1; Type < (int8)EAttackType::End; ++Type)
	{
		const FName SkillName = *FString::Printf(TEXT("WBP_PHSkillIcon%d"), Type);

		UPHCharacterSkillIconWidget* SkillIcon = Cast<UPHCharacterSkillIconWidget>(GetWidgetFromName(SkillName));

		if (SkillIcon)
		{
			SkillIconMap.Add((EAttackType)Type, SkillIcon);
		}
	}
}
