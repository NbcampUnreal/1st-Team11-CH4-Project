// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

ALever::ALever()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsLeverOn = false;
	bCanLeverOn = true;
	bCanLeverOff = false;
	bIsLocked = false;
}

FString ALever::GetInteractText()
{
	return TEXT("Lever Pull");
}

void ALever::Interact(APlayerController* Player)
{
	if (!HasAuthority())
	{
		return;
	}

	ToggleState();
}

bool ALever::CanInteract()
{
	return true;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
}

void ALever::TryChangeLeverState(const bool bNewLeverOn)
{
	if (bIsLocked || bNewLeverOn == bIsLeverOn)
	{
		return;
	}

	if (bCanLeverOn && bNewLeverOn)
	{
		bIsLeverOn = true;
		K2_OnLeverOn();
		OnLeverStateChanged.Broadcast(bIsLeverOn);
	}
	else if (bCanLeverOff && !bNewLeverOn)
	{
		bIsLeverOn = false;
		K2_OnLeverOff();
		OnLeverStateChanged.Broadcast(bIsLeverOn);
	}
}

void ALever::SetLeverLock(const bool bNewLocked)
{
	bIsLocked = bNewLocked;
}

void ALever::ToggleState()
{
	TryChangeLeverState(!bIsLeverOn);
}
