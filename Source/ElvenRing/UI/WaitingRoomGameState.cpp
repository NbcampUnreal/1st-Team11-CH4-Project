// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomGameState.h"
#include "WaitingRoomPlayerController.h"
#include <Net/UnrealNetwork.h>
#include "WaitingRoomPlayerState.h"

AWaitingRoomGameState::AWaitingRoomGameState()
{

}
//Index = 지금들어온 플레이어 인덱스이자 현재토탈 접속자. MyIndex = CurConnectedPlayerCount
void AWaitingRoomGameState::AppeareWaitingRoomPlayerCard(int Index)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        APlayerController* PC = It->Get();
        if (PC)  // 실제  접속자.DD
        {
			AWaitingRoomPlayerController* WaitingRoomPlayerController = Cast<AWaitingRoomPlayerController>(PC);
			if (WaitingRoomPlayerController)
			{
				WaitingRoomPlayerController->OpenWaitingRoom(Index);
			}   
        }
    }
}
void AWaitingRoomGameState::UpdatePlayerName(FText Name)//, AWaitingRoomPlayerState* PS
{
    if (true)return;
   
    for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        if (AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(It->Get()))
        {
            if (PC->IsLocalController() && PC->HasAuthority())
            {
                PC->Client_OnUpdatePlayerName(); // 오직 호스트 자기화면
            }
        }
    }
    bUpdatePlayerName = !bUpdatePlayerName;
   // PlayerNames.Add(Name);
   // bUpdatePlayerName = !bUpdatePlayerName;
   // OnRep_bUpdatePlayerName();
}
void AWaitingRoomGameState::OnRep_bUpdatePlayerName()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(It->Get());
        if (PC && PC->IsLocalController())//
        {
            PC->Client_OnUpdatePlayerName();
           // UE_LOG(LogTemp, Warning, TEXT("OnRep_UpdatePlayerName/ num %d"), PC->GetMyPlayerIndex());
        }
    }
    //APlayerController* PC = GetWorld()->GetFirstPlayerController();
    //if (PC && PC->IsLocalController())
    //{
    //    AWaitingRoomPlayerController* MyController = Cast<AWaitingRoomPlayerController>(PC);
    //    if (MyController)
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("MyController->OnUpdatePlayerName()"));
    //        MyController->OnUpdatePlayerName(); 
    //    }
    //}
}

void AWaitingRoomGameState::BeginPlay()
{
	Super::BeginPlay();

 /*   FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		FTimerDelegate::CreateLambda([this]() mutable
		{
            for (APlayerState* PS : PlayerArray)
            {
                AWaitingRoomPlayerState* WaitingRoomPlayerState = Cast<AWaitingRoomPlayerState>(PS);
                if (WaitingRoomPlayerState)
                {
                    UE_LOG(LogTemp, Warning, TEXT("WaitingRoomPlayerState %s"), *WaitingRoomPlayerState->PlayerName.ToString());
                }
            }
		}), 1.f, false
	);*/
}

void AWaitingRoomGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AWaitingRoomGameState, bUpdatePlayerName);
    DOREPLIFETIME(AWaitingRoomGameState, PlayerNames);

}

