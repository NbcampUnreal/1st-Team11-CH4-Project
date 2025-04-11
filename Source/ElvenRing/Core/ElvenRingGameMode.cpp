// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameMode.h"

#include <rapidjson/stream.h>

#include "ElvenringGameInstance.h"
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
		RecordPlayerDamage(Cast<APlayerController>(EventInstigator), DamagedActor, Damage);
	}
}

void AElvenRingGameMode::RecordPlayerDamage(APlayerController* EventInstigator, AActor* DamagedActor, float Damage)
{
	if (!EventInstigator || !EventInstigator->PlayerState || !DamagedActor || Damage < 0.f)
	{
		return;
	}
	
	AElvenRingPlayerState* PlayerState = EventInstigator->GetPlayerState<AElvenRingPlayerState>();
	FUniqueNetIdPtr PlayerIdPtr = PlayerState->GetUniqueId().GetUniqueNetId();
	UE_LOG(LogTemp,Display, TEXT("Player ID: %s"), *PlayerIdPtr->ToString());
	if (!PlayerIdPtr.IsValid())
	{
		UE_LOG(LogTemp,Display, TEXT("Invalid Player ID"));
		return;
	}

	TMap<UClass*, float>& DamageRecord = PlayerBossDamageRecord.FindOrAdd(PlayerIdPtr.ToSharedRef());
	UClass* BossNativeClass = GetNativeClass(DamagedActor);
	float& DamageValue = DamageRecord.FindOrAdd(BossNativeClass, 0.f);
	DamageValue += Damage;

	PlayerState->ApplyBossDamage(Damage);

	UE_LOG(LogTemp, Display, TEXT("Damage Recorded: %s -> %s, Damage: %f"), *EventInstigator->GetName(), *BossNativeClass->GetName(), DamageValue);
}

UClass* AElvenRingGameMode::GetNativeClass(const AActor* Actor)
{
	if (!Actor)
	{
		return  nullptr;
	}

	UClass* ActorClass = Actor->GetClass();
	while (ActorClass && !ActorClass->IsNative())
	{
		ActorClass = ActorClass->GetSuperClass();
	}

	return  ActorClass;
}

APlayerController* AElvenRingGameMode::GetHighestDamagePlayer(const AActor* BossActor) const
{
	if (!BossActor)
	{
		return  nullptr;
	}

	UClass* BossNativeClass = GetNativeClass(BossActor);
	APlayerController* HighestDamagePlayer = nullptr;
	float HighestDamage = 0.f;
		
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->PlayerState)
		{
			continue;
		}

		FUniqueNetIdPtr PlayerIdPtr = PC->PlayerState->GetUniqueId().GetUniqueNetId();
		const TMap<UClass*, float>* DamageRecord = PlayerBossDamageRecord.Find(PlayerIdPtr.ToSharedRef());
		if (!DamageRecord)
		{
			continue;
		}

		if (const float* DamageValue = DamageRecord->Find(BossNativeClass); DamageValue && *DamageValue > HighestDamage)
		{
			HighestDamage = *DamageValue;
			HighestDamagePlayer = PC;
		}
	}

	return HighestDamagePlayer;
}

void AElvenRingGameMode::ResetDamageRecord(const UClass* BossClass)
{
	for (auto Elem : PlayerBossDamageRecord)
	{
		TMap<UClass*, float>& DamageRecord = Elem.Value;
		if (DamageRecord.Contains(BossClass))
		{
			DamageRecord.Remove(BossClass);
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

	UE_LOG(LogTemp,Display, TEXT("Logged Player Count : %d"), PlayerBossDamageRecord.Num());
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
