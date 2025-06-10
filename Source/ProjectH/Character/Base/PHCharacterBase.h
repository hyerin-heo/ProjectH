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
	Evasion,
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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; 

	void MeshLoadCompleted();
	void UpdateMeshFromPlayerState();
	
	//State Section.
public:
	void SetActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
protected:
	void SetMontageEndDelegate();
	virtual void SetDead();
	void PlayDeadAnimation();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//프로퍼티 리플리케이션
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//Input FuncSection.
public:
	void MouseClickMove();
	void RotateToCursor();
	void SetNewLocation(FVector NewLocation);

	// Attack/Skill Action
	void SetAction();
	//Attack.
	virtual void NormalAttackUI();
	virtual void NormalAttack();
	//Evasion(회피)
	virtual void EvasionUI();
	virtual void Evasion();
	//Skill
	virtual void Skill1UI();
	virtual void Skill1();
	virtual void Skill2UI();
	virtual void Skill2();
	virtual void Skill3UI();
	virtual void Skill3();
	virtual void Skill4UI();
	virtual void Skill4();

	
	//Server RPC
	UFUNCTION(Server, Unreliable)
	void ServerRPCSetNewLocation(FVector NewLocation);
	UFUNCTION(Server, Unreliable)
	void ServerRPCSetActionTargetRotation(FRotator TargetRotation);

	UFUNCTION(Server, Unreliable)
	virtual void ServerRPCNormalAttack();
	UFUNCTION(Server, Unreliable)
	virtual void ServerRPCSkill1();
	UFUNCTION(Server, Unreliable)
	virtual void ServerRPCSkill2();

	//clientRPC
	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayAnimation(APHCharacterBase* CharacterPlayer, FName ActionName, float AnimSpeed =1.0f);

	//MulticastRPC
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCSetNewLocation(FVector NewLocation);

	//On_RepFunction
	UFUNCTION()
	void OnRep_ActionTargetRotation();
	UFUNCTION()
	void OnRep_MeshIndex();
	
protected:
	// Camera Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	//StatSection.
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UPHCharacterStatDataAsset> StatDataAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPHCharacterStatComponent> StatDataComponent;

	//Montage Section.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
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
	TObjectPtr<class UInputAction> EvasionAction;//회피(Space Bar).
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill3Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> Skill4Action;


protected:
	UPROPERTY(Replicated)
	EPlayerActionType CurrentActionType;
	
	//현재 공격/스킬을 사용할려고 누른 상태인지를 체크할 bool값.
	UPROPERTY(Replicated)
	uint8 bActioning : 1;

	UPROPERTY(ReplicatedUsing= OnRep_MeshIndex)
	int32 MeshIndex;

	UPROPERTY(ReplicatedUsing=OnRep_ActionTargetRotation)
	FRotator ActionTargetRotation;

	//CharacterMesh Section.
	//비동기식으로 로딩 하기위해 FStreamableHandle사용. 
	TSharedPtr<FStreamableHandle> MeshHandle;
	UPROPERTY(config)
	TArray<FSoftObjectPath> PlayerMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Weapon;

};
