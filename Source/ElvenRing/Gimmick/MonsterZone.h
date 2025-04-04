// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "MonsterZone.generated.h"

UCLASS()
class ELVENRING_API AMonsterZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterZone();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterZoneClearDelegate);
	FOnMonsterZoneClearDelegate OnMonsterZoneClearEvent;
	
protected:
	UFUNCTION()
	void OnMonsterDeath(AUnitBase* Unit);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnMonsterZoneClear() const;
	
	int MonsterCount;
};
