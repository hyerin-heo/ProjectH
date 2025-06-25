// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/GlobalEnum.h"
#include "GameFramework/GameMode.h"
#include "Subsystem/SkillObjectPoolSubsystem.h"
#include "PHGameMode.generated.h"

class APHCharacterBase;
class APHBossCharacterBase;
/**
 * 
 */
UCLASS()
class PROJECTH_API APHGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APHGameMode();

	//로그인 관련 함수.
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void TrySpawnPlayerPawn(APlayerController* PlayerControllerToSpawn);

	// 서버가 게임 시작 눌렀을 때.
	void StartGame();

	// CurrentStageIndex 가 지정되고 나서 호출
	void SpawnBossCharacter();

	void CharacterDied(APlayerController* DeadCharacterController);
	void CharacterReVive(APlayerController* ReViveCharacterController);
	void BossDied();

	UPROPERTY(VisibleAnywhere, Category = Subsystems)
	TSubclassOf<USkillObjectPoolSubsystem> SkillObjectPoolSubsystem;

public:
	//캐릭터 선택 함수.
	void PlayerSelectCharacter(APlayerController* InPC, EClassType ClassType);
	
protected:

	void SetPlayerMovementState(EMovementMode Mode);
	
	virtual void PostInitializeComponents() override;

	UPROPERTY()
	TArray<APlayerController*> ConnectedPlayerControllers;
	
	UPROPERTY()
	TArray<APlayerController*> DeadPlayerControllers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Boss)
	TArray<TSubclassOf<APHBossCharacterBase>> BossCharacterClass;

	UPROPERTY(EditAnywhere)
	TMap<EClassType, TSubclassOf<APHCharacterBase>> CharacterClassMap;

	// 현재 활성화된 보스 캐릭터 참조
	UPROPERTY()
	APHBossCharacterBase* ActiveBossCharacter;

	int32 CurrentStageIndex;
};
