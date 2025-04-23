// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/NormalAI/NormalMonster.h"
#include "RampageMonster.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API ARampageMonster : public ANormalMonster
{
	GENERATED_BODY()

public:
ARampageMonster();
	
	bool InstanceIsHit;
	bool InstanceIsAttack;
	bool InstanceIsDeath;

	virtual void MulticastIsHit_Implementation(bool value, FVector HitLocation, FRotator HitRotation) override;
	virtual void MulticastIsAttack_Implementation(bool value) override;
	virtual void MulticastIsDeath_Implementation(bool value) override;


	virtual void BeginPlay() override;
	virtual void OnRep_HealthChanged() override;
};
