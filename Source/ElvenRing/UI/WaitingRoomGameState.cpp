// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameState.h"
#include "WaitingRoomPlayerController.h"

AWaitingRoomGameState::AWaitingRoomGameState()
{

}
//Index = 지금들어온 플레이어 인덱스이자 현재토탈 접속자. MyIndex = CurConnectedPlayerCount
void AWaitingRoomGameState::AppeareWaitingRoomPlayerCard(int Index)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC && PC->IsLocalController())  // 실제  접속자.
        {
			AWaitingRoomPlayerController* WaitingRoomPlayerController = Cast<AWaitingRoomPlayerController>(PC);
			if (WaitingRoomPlayerController)
			{
				WaitingRoomPlayerController->OpenWaitingRoom(Index);
			}   
        }
        else
        {
        }
    }
}

void AWaitingRoomGameState::BeginPlay()
{
	Super::BeginPlay();
}

