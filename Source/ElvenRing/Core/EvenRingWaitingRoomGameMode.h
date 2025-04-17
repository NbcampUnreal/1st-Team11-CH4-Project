// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "EvenRingWaitingRoomGameMode.generated.h"

class AWaitingRoomPlayerCardsRT;
class APlayerController;
UCLASS()
class ELVENRING_API AEvenRingWaitingRoomGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AEvenRingWaitingRoomGameMode();
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<AWaitingRoomPlayerCardsRT> WaitingRoomPlayerCardsRTClass;
	//AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT;
	int32 ConnectedPlayerCount = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerController*> ConnectedPlayers;
};
