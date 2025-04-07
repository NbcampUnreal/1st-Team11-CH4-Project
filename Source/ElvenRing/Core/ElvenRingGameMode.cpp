// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameMode.h"

#include "ElvenringGameInstance.h"
#include "ElvenRing/Character/ElvenRingController.h"
#include "ElvenRing/Gimmick/EventManager.h"

AElvenRingGameMode::AElvenRingGameMode()
{
	bActorSeamlessTraveled = true;
	EventManager = CreateDefaultSubobject<UEventManager>(TEXT("EventManager"));
}

void AElvenRingGameMode::StartPlay()
{
	Super::StartPlay();

	if (UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
	{
		EventManager->Init(GameInstance->GetGameFlags());
	}
}

void AElvenRingGameMode::HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName) const
{
	if (GetNetMode() == NM_Standalone)
	{
		PlayerController->ClientTravel(LevelName, TRAVEL_Absolute);
	}
	else
	{
		GetWorld()->ServerTravel(LevelName);
		BroadcastLoadingScreen();
	}
}

void AElvenRingGameMode::BroadcastLoadingScreen() const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(*It))
		{
			PlayerController->ClientShowLoadingScreen();
		}
	}
}
