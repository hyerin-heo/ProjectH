// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Engine/StreamableManager.h"
#include "PHCharacterBase.generated.h"

UENUM()
enum class EPlayerActionType : uint8
{
	None = 0,
	NormalAttack,
	Dodge,
	Skill1,
	Skill2,
	Skill3,
	Skill4,
};

UCLASS(config=CharacterMesh)
class PROJECTH_API APHCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APHCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;
	//플레이어 스테이트가 클라이언트에 동기화 될때 호출.
	virtual void OnRep_PlayerState() override;

	void MeshLoadCompleted();
	void UpdateMeshFromPlayerState();

	//State Section.
	virtual void SetDead();
	void PlayDeadAnimation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input FuncSection.
public:
	void MouseClickMove();
	void SetNewDirection(FVector NewDirection);

	// Attack/Skill Action
	void SetAction();
	//Attack.
	virtual void NormalAttackUI();
	virtual void NormalAttack();
	
protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	//Montage Section.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	// Input Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext ;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseClickMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseClickStateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DodgeAction;//회피(Space Bar).
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill4Action;


protected:
	//어택/스킬을 몽타주에서 실행 시켜줄때 해당 FName을 활용하여 Montage_JumpToSection을 호출.
	FName PlaySectionName = "";

	EPlayerActionType CurrentActionType;
	//현재 공격/스킬을 사용할려고 누른 상태인지를 체크할 bool값.
	int8 bActioning : 1;

	//CharacterMesh Section.
	//비동기식으로 로딩 하기위해 FStreamableHandle사용. 
	TSharedPtr<FStreamableHandle> MeshHandle;
	UPROPERTY(config)
	TArray<FSoftObjectPath> PlayerMeshes;

};
