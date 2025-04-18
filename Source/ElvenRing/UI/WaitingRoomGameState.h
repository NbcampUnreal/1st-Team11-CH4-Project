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

	UPROPERTY(ReplicatedUsing = OnRep_bUpdatePlayerName)
	bool bUpdatePlayerName;

	UFUNCTION()
	void OnRep_bUpdatePlayerName();

	void AppeareWaitingRoomPlayerCard(int Index);
	void UpdatePlayerName(FText Name);

	//TMap<int32, FString> PlayerNameMap;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player Info")
	TArray<FText> PlayerNames;
protected:


	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
