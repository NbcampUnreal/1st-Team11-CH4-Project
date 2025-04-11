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
		if (AElvenRingPlayerState* PlayerState = EventInstigator->GetPlayerState<AElvenRingPlayerState>())
		{
			PlayerState->RecordPlayerDamage(DamagedActor, Damage);
		}
	}
}

ACharacter* AElvenRingGameMode::GetHighestDamageCharacter() const
{
	ACharacter* HighestDamageCharacter = nullptr;
	float HighestDamage = 0.f;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(*It))
		{
			if (ACharacter* Character = PlayerController->GetCharacter())
			{
				float Damage = PlayerController->GetPlayerState<AElvenRingPlayerState>()->GetBossDamage(Character);
				if (Damage > HighestDamage)
				{
					HighestDamage = Damage;
					HighestDamageCharacter = Character;
				}
			}
		}
	}

	return HighestDamageCharacter;
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
