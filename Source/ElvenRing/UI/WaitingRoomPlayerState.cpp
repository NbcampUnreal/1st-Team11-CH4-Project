#include "WaitingRoomPlayerState.h"
#include <Net/UnrealNetwork.h>
#include "ElvenRing/UI/WaitingRoomPlayerController.h"

AWaitingRoomPlayerState::AWaitingRoomPlayerState()
{
	PlayerName = FText::FromString(TEXT("Empty"));
}

void AWaitingRoomPlayerState::BeginPlay()
{
	Super::BeginPlay();

    FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer
	//(
	//	TimerHandle,
	//	FTimerDelegate::CreateLambda([this]() mutable
	//	{
 //    /*       APlayerController* PC = GetPlayerController();
 //           if (!PC || !PC->IsLocalController())
 //           {
 //               UE_LOG(LogTemp, Warning, TEXT("Not LocalController / AWaitingRoomPlayerState"));
 //               return;
 //           }*/
 //           if (HasAuthority())
 //               PlayerName = FText::FromString(TEXT("Host"));
 //           else
 //               PlayerName = FText::FromString(TEXT("Guest"));
	//	}), 0.1f, false
	//);
}

void AWaitingRoomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaitingRoomPlayerState, PlayerName);
}