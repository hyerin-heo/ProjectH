// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PHInGameHUDWidget.h"

#include "PHHpBarWidget.h"
#include "PHSkillBarWidget.h"
#include "Interface/Character/PHCharacterHUDInterface.h"

UPHInGameHUDWidget::UPHInGameHUDWidget(const FObjectInitializer& InObjectInitializer)
	:Super(InObjectInitializer)
{
	
}

void UPHInGameHUDWidget::InitializeHpBar(float NewMaxHp)
{
	HpBar->InitializeHpBar(NewMaxHp);
}

void UPHInGameHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UPHInGameHUDWidget::UpdateMaxHp(float NewMaxHp)
{
	HpBar->UpdateMaxHp(NewMaxHp);
}

void UPHInGameHUDWidget::SetUpSkillIcons(EAttackType InAttackType, UTexture2D* NewTexture)
{
	SkillBar->SetUpSkillIcons(InAttackType, NewTexture);
}

void UPHInGameHUDWidget::UpdateCooldownTime(EAttackType InAttackType, float CurrentCoolTime, float MaxCoolTime)
{
	SkillBar->UpdateCooldownTime(InAttackType, CurrentCoolTime, MaxCoolTime);
}

void UPHInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillBar = Cast<UPHSkillBarWidget>(GetWidgetFromName("SkillBar"));
	ensure(SkillBar);

	HpBar = Cast<UPHHpBarWidget>(GetWidgetFromName("HpBar"));
	ensure(HpBar);

	IPHCharacterHUDInterface* HUDPawn = Cast<IPHCharacterHUDInterface>(GetOwningPlayerPawn());

	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
