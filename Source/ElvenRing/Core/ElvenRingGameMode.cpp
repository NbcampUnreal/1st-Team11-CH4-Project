// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameMode.h"

AElvenRingGameMode::AElvenRingGameMode()
{
	bActorSeamlessTraveled = true;
}

void AElvenRingGameMode::HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName)
{
	if (GetNetMode() == NM_Standalone)
	{
		PlayerController->ClientTravel(LevelName, TRAVEL_Absolute);
	}
	else
	{
		GetWorld()->ServerTravel(LevelName);
	}
}
