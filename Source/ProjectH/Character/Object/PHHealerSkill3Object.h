// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PHHealerSkill3Object.generated.h"

UCLASS()
class PROJECTH_API APHHealerSkill3Object : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APHHealerSkill3Object();

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);
	UFUNCTION()
	void OnTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	void EnableSphereCollision(bool bActive);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> TriggerSphere;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> SkillEffect;

};
