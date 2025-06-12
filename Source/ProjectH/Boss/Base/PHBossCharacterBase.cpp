// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Base/PHBossCharacterBase.h"

#include "ProjectH.h"
#include "TimerManager.h"
#include "AI/PHAIController.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Physics/PHCollision.h"

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

void APHBossCharacterBase::AttackHitCheck()
{
    if (!HasAuthority())
    {
        return;
    }
    FHitResult OutHitResult;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

    const FVector Forward = GetActorForwardVector();
    const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
    const FVector End = Start + Forward * AttackRange;

    bool HitDetected = GetWorld()->SweepSingleByChannel(
        OutHitResult,
        Start,
        End,
        FQuat::Identity,
        CCHANNEL_ACTION,
        FCollisionShape::MakeSphere(AttackRadius),
        Params
    );

#if WITH_EDITOR
    // Draw attack range.
    FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
    float CapsuleHalfHeight = AttackRange * 0.5f;
    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

    DrawDebugCapsule(GetWorld(),
        CapsuleOrigin,
        CapsuleHalfHeight,
        AttackRadius,
        FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
        DrawColor,
        false,
        1.0f
        );
    
#endif
    
    if (HitDetected)
    {
        FDamageEvent DamageEvent;
        OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
    }
}

void APHBossCharacterBase::PatternHitCheck(const int32& InPatternIndex, const uint8& InStep)
{
    // @PHTODO Pattern Attack Hit
}

void APHBossCharacterBase::AttackActionRPC_Implementation()
{
    if (HasAuthority())
    {
        return;
    }
    PlayAnimMontage(ActionMontage, AttackSpeed, DefaultActionName);
    // @PHTODO Effect
}

void APHBossCharacterBase::PatternActionRPC_Implementation(const FAttackPatternDelegateWrapper& InPatternInfo, const FName InMontageName)
{
    if (HasAuthority())
    {
        return;
    }
    PlayAnimMontage(ActionMontage, InPatternInfo.PatternInfo.AttackSpeed, InMontageName);
    // @PHTODO Effect
}

void APHBossCharacterBase::PhasePatternActionRPC_Implementation(const FAttackPatternDelegateWrapper& InPhasePatternInfo, const FName InMontageName)
{
    if (HasAuthority())
    {
        return;
    }
    PlayAnimMontage(ActionMontage, InPhasePatternInfo.PatternInfo.AttackSpeed, InMontageName);
    // @PHTODO Effect
}

float APHBossCharacterBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
    class AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    const float PrevHp = HP;
    const float ActualDamage = FMath::Clamp<float>(DamageAmount, 0, DamageAmount);
    HP = PrevHp-ActualDamage;
    // @PHTODO Update UI
    if (HP <= KINDA_SMALL_NUMBER)
    {
        // @PHTODO Dead
    }
    return DamageAmount;
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
        // PH_LOG(LogPHBoss, Log, TEXT("Timer Active! if this log shown when after pattern->phase, it's right. but else, it's error. check it."));
        
        GetWorldTimerManager().ClearTimer(CoolTimeTimerHandle);
    }
    GetWorldTimerManager().SetTimer(CoolTimeTimerHandle, [&]()
    {
        // PH_LOG(LogPHBoss, Log, TEXT("Cool time End!!"));
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
        AttackRadius = DataAsset->AttackRadius;
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
