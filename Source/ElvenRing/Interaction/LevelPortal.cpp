// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelPortal.h"

#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "ElvenRing/Character/ElvenRingController.h"
#include "ElvenRing/Core/ElvenRingGameMode.h"


// Sets default values
ALevelPortal::ALevelPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
}

FString ALevelPortal::GetInteractText()
{
	return "Teleport";
}

void ALevelPortal::Interact(APlayerController* Player)
{
	// https://dev.epicgames.com/documentation/ko-kr/unreal-engine/travelling-in-multiplayer-in-unreal-engine
	if (!Player || !HasAuthority())
	{
		return;
	}

	if (AElvenRingController* PlayerController = Cast<AElvenRingController>(Player))
	{
		PlayerController->ServerRequestLevelChange(LevelPath);
	}
}

bool ALevelPortal::CanInteract()
{
	return true;
}

// Called when the game starts or when spawned
void ALevelPortal::BeginPlay()
{
	Super::BeginPlay();
	
}
