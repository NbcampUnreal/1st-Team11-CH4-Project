// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameMode.h"

#include <rapidjson/stream.h>

#include "ElvenringGameInstance.h"
#include "ElvenRingGameState.h"
#include "ElvenRingPlayerState.h"
#include "ElvenRing/Character/ElvenRingController.h"
#include "ElvenRing/Gimmick/EventManager.h"
#include "GameFramework/PlayerState.h"

AElvenRingGameMode::AElvenRingGameMode()
{
	bUseSeamlessTravel = true;
	EventManager = CreateDefaultSubobject<UEventManager>(TEXT("EventManager"));
}

void AElvenRingGameMode::RecordDamage(AController* EventInstigator, AActor* DamagedActor, float Damage)
{
	if (EventInstigator->IsA(APlayerController::StaticClass()))
	{
		if (AElvenRingGameState* ElvenRingGameState = GetGameState<AElvenRingGameState>())
		{
			ElvenRingGameState->RecordPlayerDamage(Cast<APlayerController>(EventInstigator), DamagedActor, Damage);
		}
	}
}

void AElvenRingGameMode::StartBossLap(const UClass* BossClass)
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			continue;
		}
		
		APlayerController* PC = It->Get();
		AElvenRingPlayerState* PlayerState = PC->GetPlayerState<AElvenRingPlayerState>();
		if (!PC || !PC->PlayerState)
		{
			continue;
		}

		PlayerState->ResetBossDamage();
	}
}

void AElvenRingGameMode::StopBossLap()
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			continue;
		}
		
		APlayerController* PC = It->Get();
		AElvenRingPlayerState* PlayerState = PC->GetPlayerState<AElvenRingPlayerState>();
		if (!PC || !PC->PlayerState)
		{
			continue;
		}

		PlayerState->ResetBossDamage();
	}
}

void AElvenRingGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	UE_LOG(LogTemp,Display, TEXT("PostSeamlessTravel"));
}

void AElvenRingGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
	{
		EventManager->Init(GameInstance->GetGameFlags());
	}
}

void AElvenRingGameMode::StartPlay()
{
	Super::StartPlay();

	// Start Play는 Begin Play 이훙에 호출된다.
}

void AElvenRingGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AElvenRingGameMode::HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName) const
{
	if (GetNetMode() == NM_Standalone)
	{
		// PlayerController->ClientTravel(LevelName, TRAVEL_Absolute);
		GetWorld()->ServerTravel(LevelName);
		BroadcastLoadingScreen(LevelName);
	}
	else
	{
		GetWorld()->ServerTravel(LevelName);
		BroadcastLoadingScreen(LevelName);
	}
}

void AElvenRingGameMode::BroadcastLoadingScreen(const FString& MapName) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(*It))
		{
			PlayerController->ClientShowLoadingScreen(MapName);
		}
	}
}
