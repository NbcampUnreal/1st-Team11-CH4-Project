// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomPlayerController.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/WaitingRoomPlayerCardsRT.h"
#include "ElvenRing/UI/WaitingRoomGameState.h"

AWaitingRoomPlayerController::AWaitingRoomPlayerController()
{
}

void AWaitingRoomPlayerController::SetIndex(int32 Index)
{
	 MyPlayerIndex = Index; 
}

void AWaitingRoomPlayerController::OpenWaitingRoom_Implementation(int CurConnectedPlayerCountAndMyPlayerIdx)
{
	if (!IsValid(WaitingRoomPlayerCardsRT)) 
	{
		TempCreateRT();
		//UE_LOG(LogTemp, Warning, TEXT("NULL!!! WaitingRoomPlayerCardsRT"));
		//return;
	}

	WaitingRoomPlayerCardsRT->ConnectOpenPlayerCard(CurConnectedPlayerCountAndMyPlayerIdx);
	//UE_LOG(LogTemp, Warning, TEXT("NULL!!! WaitingRoomPlayerCardsRT"));
}
void AWaitingRoomPlayerController::TempCreateRT()
{
	FString Path = TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT.BP_WaitingRoomPlayerCardsRT_C");
	UClass* WaitingRoomPlayerCardsRTClass = StaticLoadClass(AWaitingRoomPlayerCardsRT::StaticClass(), nullptr, *Path);
	if (WaitingRoomPlayerCardsRTClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
		FRotator Rotation = FRotator::ZeroRotator;
		WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
	}

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
}
void AWaitingRoomPlayerController::BeginPlay()
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not LocalController / MyPlayerIndex : %d"), MyPlayerIndex);
		return;
	}

	FString Path = TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT.BP_WaitingRoomPlayerCardsRT_C");
	UClass* WaitingRoomPlayerCardsRTClass = StaticLoadClass(AWaitingRoomPlayerCardsRT::StaticClass(), nullptr, *Path);
	if (WaitingRoomPlayerCardsRTClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
		FRotator Rotation = FRotator::ZeroRotator;
		WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
	}

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
}
