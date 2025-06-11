// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHWarriorSkill3Object.generated.h"

UCLASS()
class PROJECTH_API APHWarriorSkill3Object : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHWarriorSkill3Object();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class USphereComponent* GetTriggerSphere() {return TriggerSphere;}

	UFUNCTION()
	// 외부에서 데미지 및 시전자 지정
	void InitializeSkill(float InDamage, AActor* InInstigator);

	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> TriggerSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> FireEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> FloorEffect;

	

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UStaticMeshComponent> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= State, Meta = (AllowPrivateAccess = "true"))
	float LifeTime = 1.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = "true"))
	float Damage = 0.0f;

	// 시전자
	UPROPERTY()
	TObjectPtr<AActor> SkillInstigator;

};
