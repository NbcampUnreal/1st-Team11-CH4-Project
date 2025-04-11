// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ElvenRingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AElvenRingPlayerState();

	void ResetBossDamage();
	float GetCurrentBossDamage() const { return CurrentBossDamage; }
	void ApplyBossDamage(float Damage);
protected:
	virtual void BeginPlay() override;
	
	float CurrentBossDamage;

	virtual void CopyProperties(APlayerState* PlayerState) override;
};
