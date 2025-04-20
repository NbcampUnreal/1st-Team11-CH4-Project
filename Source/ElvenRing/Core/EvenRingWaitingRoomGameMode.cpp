// Fill out your copyright notice in the Description page of Project Settings.


#include "EvenRingWaitingRoomGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/WaitingRoomPlayerCardsRT.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "ElvenRing/UI/WaitingRoomPlayerController.h"
#include "ElvenRing/UI/WaitingRoomGameState.h"
#include "ElvenRing/UI/WaitingRoomPlayerState.h"

AEvenRingWaitingRoomGameMode::AEvenRingWaitingRoomGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerStateClass = AWaitingRoomPlayerState::StaticClass();

	//static ConstructorHelpers::FClassFinder<AWaitingRoomPlayerCardsRT> BP_WaitingRoomPlayerCardsRT(TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT"));
	//if (BP_WaitingRoomPlayerCardsRT.Succeeded())
	//	WaitingRoomPlayerCardsRTClass = BP_WaitingRoomPlayerCardsRT.Class;
}

void AEvenRingWaitingRoomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options,ErrorMessage);

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
	//FRotator Rotation = FRotator::ZeroRotator;

	//WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);

	//UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	//if (EGameInstance)
	//	EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
}

void AEvenRingWaitingRoomGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ConnectedPlayers.Add(NewPlayer);

	FString PlayerName = TEXT("Guest");
	if (ConnectedPlayers.Num() == 1) //처음접속
		PlayerName = TEXT("Host");
	AWaitingRoomPlayerController* WaitingRoomPlayerController = Cast<AWaitingRoomPlayerController>(NewPlayer);
	WaitingRoomPlayerController->MyPlayerIndex = ConnectedPlayerCount;
	ConnectedPlayerCount++;

	int TempConnectedPlayerCount = ConnectedPlayerCount;

	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([this, TempConnectedPlayerCount, NewPlayer]()
	{
		AWaitingRoomGameState* WaitingRoomGameState = Cast<AWaitingRoomGameState>(GetWorld()->GetGameState());
		WaitingRoomGameState->AppeareWaitingRoomPlayerCard(TempConnectedPlayerCount);
		UE_LOG(LogTemp, Warning, TEXT("PostLogin ConnectedPlayerCount %d"), TempConnectedPlayerCount);
	}), 0.2f, false); 
}
void AEvenRingWaitingRoomGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
	FRotator Rotation = FRotator::ZeroRotator;

	WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
		*/
}