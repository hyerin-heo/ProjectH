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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// UPROPERTY(VisibleDefaultsOnly)
	// TObjectPtr<USceneComponent> Root;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	// TObjectPtr<class UNiagaraComponent> Effect;
	//
	// UPROPERTY(EditDefaultsOnly, Category= State)
	// float LifeTime = 1.5f;

};
