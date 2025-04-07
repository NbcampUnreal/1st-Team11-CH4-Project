// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagTrigger.h"

#include "ElvenRing/Core/ElvenRingGameMode.h"
#include "ElvenRing/Gimmick/EventManager.h"
#include "ElvenRing/Gimmick/GameFlag.h"


FString AFlagTrigger::GetInteractText()
{
	return TEXT("Lever Pull");
}

void AFlagTrigger::Interact(APlayerController* Player)
{
	UE_LOG(LogTemp,Display, TEXT("Lever Interact"));
	if (AElvenRingGameMode* GameMode = Cast<AElvenRingGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (GameMode->GetEventManager())
		{
			GameMode->GetEventManager()->SetGameFlag(GameFlagAsset->FlagName, TargetCondition);
			OnEventTriggered.Broadcast();
		}
	}
}

bool AFlagTrigger::CanInteract()
{
	return  true;
}

// Sets default values
AFlagTrigger::AFlagTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFlagTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}
