// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Base/PHBossCharacterBase.h"

#include "ProjectH.h"
#include "TimerManager.h"
#include "AI/PHAIController.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APHBossCharacterBase::APHBossCharacterBase()
{
    AIControllerClass = APHAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // init
    CurrentPatternIndex = MAX_int32;
    bIsPhaseTimerDone = false;
}

void APHBossCharacterBase::PhaseLevelChanged(const uint8& OldPhase, const uint8& NewPhase)
{
    PH_LOG(LogPHBoss, Log, TEXT("Change Phase Level : %s"), *FString::FromInt(CurrentPhaseLevel.GetValue()));
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
    if (bIsPhaseTimerDone)
    {
        bIsPhaseTimerDone = false;
        return true;
    }
    if (const FBossPhaseInfo* FoundInfo = PhaseMap.Find(CurrentPhaseLevel + 1))
    {
        switch (FoundInfo->TriggerType)
        {
        case EBossPhaseTriggerType::HealthPercent:
            {
                if (GetHpPercent() < FoundInfo->TriggerValue)
                {
                    CurrentPhaseLevel.SetValue(CurrentPhaseLevel.GetValue() + 1);
                    return true;
                }
                break;
            }
        case EBossPhaseTriggerType::Timer:
            {
                if (!GetWorldTimerManager().IsTimerActive(PhaseTimerHandle))
                {
                    GetWorldTimerManager().SetTimer(PhaseTimerHandle, [&]()
                    {
                        CurrentPhaseLevel.SetValue(CurrentPhaseLevel.GetValue() + 1);
                        bIsPhaseTimerDone = true;
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
    PlayAnimMontage(ActionMontage, AttackSpeed, DefaultActionName);
    
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::AttackActionEnd);
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);

    // client에 attack 전송
    AttackActionRPC();
}

void APHBossCharacterBase::AttackActionEnd(UAnimMontage* AnimMontage, bool bArg)
{
    OnAttackFinished.ExecuteIfBound();
}

void APHBossCharacterBase::PatternAttackActionEnd(UAnimMontage* AnimMontage, bool bArg)
{
    SetCoolTime();
    OnPatternAttackFinished.ExecuteIfBound();
}

void APHBossCharacterBase::PatternAction()
{
    checkf(AttackPatternActions.Num() > 0, TEXT("Special attack pattern action is empty!"));

    int32 Index = -1;
    do
    {
        Index = FMath::RandRange(0, AttackPatternActions.Num() - 1);
    }
    while (Index == CurrentPatternIndex && AttackPatternActions.Num() > 1);

    FName MontageName = FName(*FString::Printf(TEXT("%s%d"),*PatternActionName.ToString(), Index));
    PlayAnimMontage(ActionMontage, AttackPatternActions[Index].PatternInfo.AttackSpeed, MontageName);
    // AnimInstance->Montage_Play(ActionMontage, AttackSpeed);
    AttackPatternActions[Index].ItemDelegate.Execute();
    // save cool time
    CurrentPatternCoolTime = AttackPatternActions[Index].PatternInfo.CoolTime;
    // save pattern index
    CurrentPatternIndex = Index;

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::PatternAttackActionEnd);
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);

    // client에 attack 전송
    PatternActionRPC(AttackPatternActions[Index], MontageName);
}

void APHBossCharacterBase::PhasePatternAction()
{
    checkf(AttackPatternActions.Num() > 0, TEXT("Special attack pattern action is empty!"));

    auto Phase = AttackPatternActions.Last();
    
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    // AnimInstance->Montage_Play(ActionMontage, Phase.PatternInfo.AttackSpeed);
    FName MontageName = FName(*FString::Printf(TEXT("%s%d"),*PatternActionName.ToString(), AttackPatternActions.Num() - 1));
    PlayAnimMontage(ActionMontage, Phase.PatternInfo.AttackSpeed, MontageName);
    Phase.ItemDelegate.Execute();
    // save cool time
    CurrentPatternCoolTime = Phase.PatternInfo.CoolTime;
    // save pattern index
    CurrentPatternIndex = Phase.PatternInfo.PatternIndex;

    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &APHBossCharacterBase::PatternAttackActionEnd);
    AnimInstance->Montage_SetEndDelegate(EndDelegate, ActionMontage);
    
    // client에 attack 전송
    PhasePatternActionRPC(Phase, MontageName);
}

void APHBossCharacterBase::AttackActionRPC_Implementation()
{
    if (HasAuthority())
    {
        return;
    }
    PH_LOG(LogPHBoss, Log, TEXT("APHBossCharacterBase::AttackActionRPC"));
    PlayAnimMontage(ActionMontage, AttackSpeed, DefaultActionName);
    // @PHTODO Effect
}

void APHBossCharacterBase::PatternActionRPC_Implementation(const FAttackPatternDelegateWrapper& InPatternInfo, const FName InMontageName)
{
    if (HasAuthority())
    {
        return;
    }
    PH_LOG(LogPHBoss, Log, TEXT("APHBossCharacterBase::PatternActionRPC MontageName -> %s, AttackSpeed -> %f"), *InMontageName.ToString(), InPatternInfo.PatternInfo.AttackSpeed);
    PlayAnimMontage(ActionMontage, InPatternInfo.PatternInfo.AttackSpeed, InMontageName);
    // @PHTODO Effect
}

void APHBossCharacterBase::PhasePatternActionRPC_Implementation(const FAttackPatternDelegateWrapper& InPhasePatternInfo, const FName InMontageName)
{
    if (HasAuthority())
    {
        return;
    }
    PH_LOG(LogPHBoss, Log, TEXT("APHBossCharacterBase::PhasePatternActionRPC"));
    PlayAnimMontage(ActionMontage, InPhasePatternInfo.PatternInfo.AttackSpeed, InMontageName);
    // @PHTODO Effect
}

void APHBossCharacterBase::OnRep_MaxHP()
{
    // @PHTODO 최대 체력 변경됐을 때, UI업데이트 필요.
}

void APHBossCharacterBase::OnRep_HP()
{
    // @PHTODO 체력 변경됐을 때, UI업데이트 필요.
}

bool APHBossCharacterBase::IsCoolTime()
{
    return GetWorldTimerManager().IsTimerActive(CoolTimeTimerHandle);
}

void APHBossCharacterBase::SetCoolTime()
{
    if (GetWorldTimerManager().IsTimerActive(CoolTimeTimerHandle))
    {
        PH_LOG(LogPHBoss, Log, TEXT("Timer Active! if this log shown when after pattern->phase, it's right. but else, it's error. check it."));
        
        GetWorldTimerManager().ClearTimer(CoolTimeTimerHandle);
    }
    GetWorldTimerManager().SetTimer(CoolTimeTimerHandle, [&]()
    {
        PH_LOG(LogPHBoss, Log, TEXT("Cool time End!!"));
    },CurrentPatternCoolTime , false);
}

// Called when the game starts or when spawned
void APHBossCharacterBase::BeginPlay()
{
    Super::BeginPlay();
    CurrentPhaseLevel.OnValueChanged.BindUObject(this, &APHBossCharacterBase::PhaseLevelChanged);
    CurrentPhaseLevel = 1;
    bIsPhaseTimerDone = false;

    checkf(DataAsset, TEXT("DataAsset must be exist. \nCall Stack : %s"), ANSI_TO_TCHAR(__FUNCTION__));

    if (DataAsset)
    {
        MaxHP = DataAsset->MaxHp;
        HP = MaxHP;
        PhaseMap = DataAsset->PhaseMap;
        DetectionRadius = DataAsset->DetectionRadius;
        Speed = DataAsset->Speed;
        AttackDamage = DataAsset->AttackDamage;
        AttackRange = DataAsset->AttackRange;
        AttackSpeed = DataAsset->AttackSpeed;
        Armor = DataAsset->Armor;

        GetCharacterMovement()->MaxWalkSpeed = Speed;
    }
}

void APHBossCharacterBase::SetAIAttackDelegate(const FAIAttackFinished& InOnAttackFinished)
{
    OnAttackFinished = InOnAttackFinished;
}

void APHBossCharacterBase::SetAIPatternAttackDelegate(const FAIPatternAttackFinished& InOnPatternAttackFinished)
{
    OnPatternAttackFinished = InOnPatternAttackFinished;
}

void APHBossCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APHBossCharacterBase, MaxHP); 
    DOREPLIFETIME(APHBossCharacterBase, HP);
}
