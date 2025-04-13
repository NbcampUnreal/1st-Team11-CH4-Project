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
	bAfterSeamlessTravel = false;
	bIsAllPlayersReady = false;
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

ACharacter* AElvenRingGameMode::GetHighestDamageCharacter(const AActor* BossActor) const
{
	ACharacter* HighestDamageCharacter = nullptr;
	float HighestDamage = 0.f;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(*It))
		{
			if (ACharacter* Character = PlayerController->GetCharacter())
			{
				float Damage = PlayerController->GetPlayerState<AElvenRingPlayerState>()->GetBossDamage(BossActor);
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

	UE_LOG(LogTemp,Display, TEXT("PostSeamlessTravel : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);

	bAfterSeamlessTravel = true;
	// 가능한 지는 모르겠지만 Client가 이미 Seamless Travel을 완료했을 경우
	if (NumTravellingPlayers == 0)
	{
		OnAllPlayersReady();
	}
}

void AElvenRingGameMode::StartToLeaveMap()
{
	// Match State을 LeavingMap으로 변경
	Super::StartToLeaveMap();

	// 이 값을 이용해서 Get Num Players를 구할 수 있을 것이라 판단했다.
	// 하지만, Game Mode는 재생성되므로 이 값을 이용할 수 없다.
	// 로딩 스크린을 이용하거나 아니면 Player State를 이용하거나 Game Instance를 경유해서 데이터 넘기는 방법을 고려할 필요가 있다.
	UE_LOG(LogTemp,Display,TEXT("StartToLeaveMap : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
}

void AElvenRingGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	// Seamless Travel 이후 적절한 PlayerController를 생성
	Super::HandleSeamlessTravelPlayer(C);

	UE_LOG(LogTemp,Display, TEXT("HandleSeamlessTravelPlayer : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
	if (bAfterSeamlessTravel && NumTravellingPlayers == 0)
	{
		OnAllPlayersReady();
	}
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
}

void AElvenRingGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Display, TEXT("AElvenRingGameMode::BeginPlay() / Num Players : %d / Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
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

bool AElvenRingGameMode::AreAllPlayersReady() const
{
	return bIsAllPlayersReady;
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

void AElvenRingGameMode::OnAllPlayersReady()
{
	bIsAllPlayersReady = true;

	UE_LOG(LogTemp,Display, TEXT("OnAllPlayersReady() / Num Players : %d / Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);

	OnAllPlayersReadyDelegate.Broadcast();
}
