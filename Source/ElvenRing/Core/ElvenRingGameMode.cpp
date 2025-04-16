// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameMode.h"

#include "ElvenringGameInstance.h"
#include "ElvenRingGameState.h"
#include "ElvenRingPlayerState.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "ElvenRing/Character/ElvenRingController.h"
#include "ElvenRing/Gimmick/EventManager.h"

AElvenRingGameMode::AElvenRingGameMode()
{
	bUseSeamlessTravel = true;
	EventManager = CreateDefaultSubobject<UEventManager>(TEXT("EventManager"));
	bAfterSeamlessTravel = false;
	bIsAllPlayerLoadMap = false;
	LoadingTimeOutTime = 60.0f;
	PlayerReadyCount = 0;
	bHasPlayersReady = false;
	TravelDelayTime = 0.3f;
	
	bDelayedStart = true;
}

void AElvenRingGameMode::RecordDamage(AController* EventInstigator, AActor* DamagedActor, float Damage)
{
	if (!IsValid(EventInstigator))
	{
		UE_LOG(LogTemp,Warning, TEXT("Record Damage : EventInstigator is not valid. Damage may not be recorded."));
		return;
	}
	
	if (EventInstigator->IsA(APlayerController::StaticClass()))
	{
		if (AElvenRingPlayerState* PlayerState = EventInstigator->GetPlayerState<AElvenRingPlayerState>())
		{
			PlayerState->RecordPlayerDamage(DamagedActor, Damage);
		}
	}

	if (AElvenRingCharacter* Character = Cast<AElvenRingCharacter>(DamagedActor))
	{
		if (AElvenRingPlayerState* PlayerState = Character->GetPlayerState<AElvenRingPlayerState>())
		{
			PlayerState->RecordPlayerDamageTaken(DamagedActor, Damage);
		}
	}
}

ACharacter* AElvenRingGameMode::GetHighestDamageCharacter(const AActor* BossActor) const
{
	ACharacter* HighestDamageCharacter = nullptr;
	float HighestDamage = -1.f;

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

void AElvenRingGameMode::RecordInvincible(AController* EventInstigator, AUnitBase* DamagedActor, float Damage)
{
	if (!IsValid(EventInstigator))
	{
		UE_LOG(LogTemp,Warning, TEXT("Record Damage : EventInstigator is not valid. Damage may not be recorded."));
		return;
	}

	if (AElvenRingCharacter* Character = Cast<AElvenRingCharacter>(DamagedActor))
	{
		if (AElvenRingPlayerState* PlayerState = Character->GetPlayerState<AElvenRingPlayerState>())
		{
			PlayerState->RecordDodge(DamagedActor, Damage);
		}
	}
}

void AElvenRingGameMode::HandlePlayerDeath(AController* DeadController)
{
	if (!IsValid(DeadController))
	{
		return;
	}

	if (!DeadController->GetPawn())
	{
		DeadController->UnPossess();
	}

	if (AElvenRingPlayerState* PlayerState = DeadController->GetPlayerState<AElvenRingPlayerState>())
	{
		PlayerState->AddRespawnCount();
	}

	RestartPlayer(DeadController);
}

void AElvenRingGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	UE_LOG(LogTemp,Display, TEXT("PostSeamlessTravel : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);

	bAfterSeamlessTravel = true;
	// 가능한 지는 모르겠지만 Client가 이미 Seamless Travel을 완료했을 경우
	if (NumTravellingPlayers == 0)
	{
		OnAllPlayerLoadMap();
		return;
	}

	GetWorldTimerManager().SetTimer(
		LoadingTimeOutHandle,
		this,
		&AElvenRingGameMode::OnAllPlayerLoadMap,
		LoadingTimeOutTime,
		false
	);
}

void AElvenRingGameMode::StartToLeaveMap()
{
	// Match State을 LeavingMap으로 변경
	Super::StartToLeaveMap();

	// 이 값을 이용해서 Get Num Players를 구할 수 있을 것이라 판단했다.
	// 하지만, Game Mode는 재생성되므로 이 값을 이용할 수 없다.
	// 로딩 스크린을 이용하거나 아니면 Player State를 이용하거나 Game Instance를 경유해서 데이터 넘기는 방법을 고려할 필요가 있다.
	UE_LOG(LogTemp,Display,TEXT("StartToLeaveMap : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		AElvenRingController* PlayerController = Cast<AElvenRingController>(*Iterator);
		if (!IsValid(PlayerController))
		{
			continue;
		}
		AElvenRingPlayerState* PlayerState = Cast<AElvenRingPlayerState>(PlayerController->PlayerState);
		if (!IsValid(PlayerState))
		{
			continue;
		}
		AElvenRingCharacter* Character = Cast<AElvenRingCharacter>(PlayerController->GetCharacter());
		if (!IsValid(Character))
		{
			continue;
		}

		PlayerState->SaveCharacterStatus(Character);
	}
}

void AElvenRingGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	// Seamless Travel 이후 적절한 PlayerController를 생성
	Super::HandleSeamlessTravelPlayer(C);

	UE_LOG(LogTemp,Display, TEXT("HandleSeamlessTravelPlayer : Num Of Players : %d, Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
	if (bAfterSeamlessTravel && NumTravellingPlayers == 0)
	{
		OnAllPlayerLoadMap();
	}
}

void AElvenRingGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

APawn* AElvenRingGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	APawn* NewPawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);

	if (AElvenRingPlayerState* PlayerState = NewPlayer->GetPlayerState<AElvenRingPlayerState>(); PlayerState->HasSaved())
	{
		if (AElvenRingCharacter* Character = Cast<AElvenRingCharacter>(NewPawn))
		{
			PlayerState->LoadCharacterStatus(Character);
		}
	}

	return NewPawn;
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

	// 이 시점에서도 bAfterSeamlessTravel이 false일 경우, Seamless Travel이 아니라 처음 맵을 연 상태이다.
	if (!bAfterSeamlessTravel)
	{
		bDelayedStart = false;
	}
}

void AElvenRingGameMode::HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName)
{
	if (GetNetMode() == NM_Standalone)
	{
		// PlayerController->ClientTravel(LevelName, TRAVEL_Absolute);
		BroadcastLoadingScreen(LevelName, TravelDelayTime);

		FTimerDelegate ServerTravelDelegate;
		ServerTravelDelegate.BindLambda([this, LevelName]()
		{
			GetWorld()->ServerTravel(LevelName);
		});
		GetWorldTimerManager().SetTimer(
			TravelDelayHandle,
			ServerTravelDelegate,
			TravelDelayTime,
			false
		);
	}
	else
	{
		BroadcastLoadingScreen(LevelName, TravelDelayTime);

		FTimerDelegate ServerTravelDelegate;
		ServerTravelDelegate.BindLambda([this, LevelName]()
		{
			GetWorld()->ServerTravel(LevelName);
		});
		GetWorldTimerManager().SetTimer(
			TravelDelayHandle,
			ServerTravelDelegate,
			TravelDelayTime,
			false
		);
	}
}

bool AElvenRingGameMode::AreAllPlayersReady() const
{
	return bHasPlayersReady;
}

void AElvenRingGameMode::HandleNetworkReady(AElvenRingController* ElvenRingController)
{
	// 원래라면 잘못된 중복 입력을 처리하기 위해 Controller를 저장해야 하지만 구현 간단성을 위해 생략한다.
	// 추후에 버그가 있을 경우 Controller를 저장해서 확인한다.
	if (bHasPlayersReady)
	{
		return;
	}

	PlayerReadyCount++;
	UE_LOG(LogTemp,Display, TEXT("HandleNetworkReady() / Player Ready Count : %d / Player Name : %s"), PlayerReadyCount, *ElvenRingController->GetName());
	// Start Play 이후이기 때문에 Pending Players가 없다.	
	if (PlayerReadyCount >= GetNumPlayers())
	{
		OnAllPlayerReady();
	}
}

void AElvenRingGameMode::BroadcastLoadingScreen(const FString& MapName, float FadeOutTime) const
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(*It))
		{
			PlayerController->ClientShowLoadingScreen(MapName, FadeOutTime);
		}
	}
}

void AElvenRingGameMode::OnAllPlayerLoadMap()
{
	bIsAllPlayerLoadMap = true;
	GetWorldTimerManager().ClearTimer(LoadingTimeOutHandle);
	bDelayedStart = false;

	UE_LOG(LogTemp,Display, TEXT("OnAllPlayerLoadMap() / Num Players : %d / Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
}

void AElvenRingGameMode::OnAllPlayerReady()
{
	if (bHasPlayersReady)
	{
		return;
	}
	
	UE_LOG(LogTemp,Display, TEXT("OnAllPlayersReady() / Num Players : %d / Traveling Players : %d"), GetNumPlayers(), NumTravellingPlayers);
	bHasPlayersReady = true;
	OnAllPlayersReadyDelegate.Broadcast();
}
