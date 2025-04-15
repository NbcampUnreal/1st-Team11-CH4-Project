// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EvenRingWaitingRoomGameMode.generated.h"

class AWaitingRoomPlayerCardsRT;

UCLASS()
class ELVENRING_API AEvenRingWaitingRoomGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AEvenRingWaitingRoomGameMode();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<AWaitingRoomPlayerCardsRT> WaitingRoomPlayerCardsRTClass;
	AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT;
};
