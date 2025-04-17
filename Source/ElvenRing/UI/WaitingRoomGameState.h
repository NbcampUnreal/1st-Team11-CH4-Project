// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WaitingRoomGameState.generated.h"


UCLASS()
class ELVENRING_API AWaitingRoomGameState : public AGameState
{
	GENERATED_BODY()
public:
	AWaitingRoomGameState();

	void AppeareWaitingRoomPlayerCard(int Index); 
protected:
	virtual void BeginPlay() override;
};
