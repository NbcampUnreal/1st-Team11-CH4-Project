// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomPlayerState.h"

AWaitingRoomPlayerState::AWaitingRoomPlayerState()
{
	PlayerName = TEXT("Empty");
}

void AWaitingRoomPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
		PlayerName = TEXT("Host");
	else
		PlayerName = TEXT("Guest");
}

void AWaitingRoomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaitingRoomPlayerState, PlayerName);
}