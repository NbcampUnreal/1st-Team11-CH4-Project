// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ElvenRingGameState.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingGameState : public AGameState
{
	GENERATED_BODY()

public:
	AElvenRingGameState();
	
	virtual void BeginPlay() override;

	
};
