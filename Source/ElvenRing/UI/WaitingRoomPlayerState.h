// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "WaitingRoomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AWaitingRoomPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AWaitingRoomPlayerState();
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	FString PlayerName;
protected:

	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

};
