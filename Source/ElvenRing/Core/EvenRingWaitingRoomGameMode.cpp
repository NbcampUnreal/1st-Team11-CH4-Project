// Fill out your copyright notice in the Description page of Project Settings.


#include "EvenRingWaitingRoomGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"

AEvenRingWaitingRoomGameMode::AEvenRingWaitingRoomGameMode()
{
	DefaultPawnClass = nullptr;
}
void AEvenRingWaitingRoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld());
	}
}