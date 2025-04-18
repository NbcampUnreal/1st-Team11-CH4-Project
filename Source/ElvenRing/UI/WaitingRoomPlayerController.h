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

	UPROPERTY(Replicated)
	int32 MyPlayerIndex = -1;

	UFUNCTION(Client, Reliable)
	void SetIndex(int32 Index);

	UFUNCTION(Client, Reliable)
	void OpenWaitingRoom(int PlayerIdx);

	UFUNCTION(Client, Reliable)
	void Client_OnUpdatePlayerName();

	void TempCreateRT();
	int32 GetMyPlayerIndex() { return MyPlayerIndex; }

	UFUNCTION(Server, Reliable)
	void Server_SetName1(const FText& Name);
protected:
	

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<AWaitingRoomPlayerCardsRT> WaitingRoomPlayerCardsRTClass;
	AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT;


	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	
};
