// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/GlobalEnum.h"
#include "GameFramework/PlayerState.h"
#include "PHPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTH_API APHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APHPlayerState();

	FORCEINLINE void SetSelectedClass(EClassType InClassType) {SelectedClass = InClassType;}

	UClass* GetSelectedClass() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;


public:
	UFUNCTION()
	void OnRep_SelectedClass();

private:
	UPROPERTY(ReplicatedUsing=OnRep_SelectedClass)
	EClassType SelectedClass = EClassType::None;
	
};
