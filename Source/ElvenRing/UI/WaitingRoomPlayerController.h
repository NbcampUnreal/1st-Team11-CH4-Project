// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WaitingRoomPlayerController.generated.h"

class AWaitingRoomPlayerCardsRT;

UCLASS()
class ELVENRING_API AWaitingRoomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWaitingRoomPlayerController();

	void SetIndex(int32 Index);

	UFUNCTION(Client, Reliable)
	void OpenWaitingRoom(int PlayerIdx);

	void TempCreateRT();
	
protected:
	int32 MyPlayerIndex = -1;
	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<AWaitingRoomPlayerCardsRT> WaitingRoomPlayerCardsRTClass;
	AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT;


	virtual void BeginPlay() override;

private:
	
};
