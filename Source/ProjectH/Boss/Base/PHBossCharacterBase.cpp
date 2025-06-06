// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Base/PHBossCharacterBase.h"

#include "ProjectH.h"
#include "TimerManager.h"
#include "AI/PHAIController.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APHBossCharacterBase::APHBossCharacterBase()
{
    AIControllerClass = APHAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    CurrentPhaseLevel.OnValueChanged.BindUObject(this, &APHBossCharacterBase::PhaseLevelChanged);

    // init
    CurrentPatternIndex = MAX_int32;
}

void APHBossCharacterBase::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
{
    PH_LOG(LogPHBoss, Log, TEXT("Change Phase Level : %hhu"), CurrentPhaseLevel.GetValue());
}

float APHBossCharacterBase::GetDetectionRadius()
{
    return DetectionRadius;
}

float APHBossCharacterBase::GetAttackRange()
{
    return AttackRange;
}

float APHBossCharacterBase::GetSpeed()
{
    return Speed;
}

float APHBossCharacterBase::GetAttackSpeed()
{
    return AttackSpeed;
}

float APHBossCharacterBase::GetArmor()
{
    return Armor;
}

bool APHBossCharacterBase::IsPhase()
{
    if (const FBossPhaseInfo* FoundInfo = PhaseMap.Find(CurrentPhaseLevel + 1))
    {
        switch (FoundInfo->TriggerType)
        {
        case EBossPhaseTriggerType::HealthPercent:
            {
                if (GetHpPercent() < FoundInfo->TriggerValue)
                {
                    // @PHTODO
                    // 레벨 변경 다시 볼 것. AI와 관련하여
                    // bisPhasePattern으로 flag값 둘 것
                    CurrentPhaseLevel.SetValue(CurrentPhaseLevel.GetValue() + 1);
                    return true;
                }
                break;
            }
        case EBossPhaseTriggerType::Timer:
            {
                if (!PhaseTimerHandle.IsValid())
                {
                    GetWorldTimerManager().SetTimer(PhaseTimerHandle, [&]()
                    {
                        // @PHTODO
                        // 레벨 변경 다시 볼 것. AI와 관련하여
                        // bisPhasePattern으로 flag값 둘 것
                        CurrentPhaseLevel.SetValue(CurrentPhaseLevel.GetValue() + 1);
                        return true;
                    }, FoundInfo->TriggerValue, false);
                }
                break;
            }
        }
    }
    return false;
}

void APHBossCharacterBase::AttackAction()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ActionMontage, AttackSpeed);

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::AttackActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
}

void APHBossCharacterBase::AttackActionEnd(UAnimMontage* AnimMontage, bool bArg)
{
    OnAttackFinished.Execute();
}

void APHBossCharacterBase::PatternAction()
{
    checkf(AttackPatternActions.Num() > 0, TEXT("Special attack pattern action is empty!"));

    int32 Index = -1;
    do
    {
        Index = FMath::RandRange(0, AttackPatternActions.Num() - 1);
    }
    while (Index != CurrentPatternIndex);
    
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ActionMontage, AttackSpeed);
    AttackPatternActions[Index].ItemDelegate.Execute();
    // save cool time
    CurrentPatternCoolTime = AttackPatternActions[Index].PatternInfo.CoolTime;
    // save pattern index
    CurrentPatternIndex = Index;

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::AttackActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
}

void APHBossCharacterBase::PhasePatternAction()
{
    checkf(AttackPatternActions.Num() > 0, TEXT("Special attack pattern action is empty!"));

    auto Phase = AttackPatternActions.Last();
    
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_Play(ActionMontage, Phase.PatternInfo.AttackSpeed);
    Phase.ItemDelegate.Execute();
    // save cool time
    CurrentPatternCoolTime = Phase.PatternInfo.CoolTime;
    // save pattern index
    CurrentPatternIndex = Phase.PatternInfo.PatternIndex;

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::AttackActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
}

bool APHBossCharacterBase::IsCoolTime()
{
    return CoolTimeTimerHandle.IsValid();
}

void APHBossCharacterBase::SetCoolTime()
{
    GetWorldTimerManager().SetTimer(CoolTimeTimerHandle, [&]()
    {
        PH_LOG(LogPHBoss, Log, TEXT("Cool time End!!"));
    },CurrentPatternCoolTime , false);
}

// Called when the game starts or when spawned
void APHBossCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    CurrentPhaseLevel = 1;

    checkf(DataAsset, TEXT("DataAsset must be exist. \nCall Stack : %s"), ANSI_TO_TCHAR(__FUNCTION__));

    if (DataAsset)
    {
        MaxHP = DataAsset->MaxHp;
        HP = MaxHP;
        PhaseMap = DataAsset->PhaseMap;
        DetectionRadius = DataAsset->DetectionRadius;
        Speed = DataAsset->Speed;
        AttackRange = DataAsset->AttackRange;
        AttackSpeed = DataAsset->AttackSpeed;
        Armor = DataAsset->Armor;
        bIsUnableToAttack = false;
    }
}

void APHBossCharacterBase::SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished)
{
    OnAttackFinished = InOnAttackFinished;
}
