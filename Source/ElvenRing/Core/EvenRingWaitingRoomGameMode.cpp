// Fill out your copyright notice in the Description page of Project Settings.


#include "EvenRingWaitingRoomGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/WaitingRoomPlayerCardsRT.h"

AEvenRingWaitingRoomGameMode::AEvenRingWaitingRoomGameMode()
{
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<AWaitingRoomPlayerCardsRT> BP_WaitingRoomPlayerCardsRT(TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT"));
	if (BP_WaitingRoomPlayerCardsRT.Succeeded())
		WaitingRoomPlayerCardsRTClass = BP_WaitingRoomPlayerCardsRT.Class;
}
void AEvenRingWaitingRoomGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
	FRotator Rotation = FRotator::ZeroRotator;

	WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
	
	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
	}

	int32 dsfsdf = 0;



}