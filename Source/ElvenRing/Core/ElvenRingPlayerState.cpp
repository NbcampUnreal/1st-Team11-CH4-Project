// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingPlayerState.h"

#include "Net/UnrealNetwork.h"

AElvenRingPlayerState::AElvenRingPlayerState()
{
	CurrentBossDamage = 0.0f;
}

void AElvenRingPlayerState::ResetBossDamage()
{
	CurrentBossDamage = 0.0f;
}

void AElvenRingPlayerState::ApplyBossDamage(float Damage)
{
	if (Damage < 0.f)
	{
		return;
	}
	CurrentBossDamage += Damage;
}

void AElvenRingPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp,Display, TEXT("PlayerState BeginPlay"));
}

void AElvenRingPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AElvenRingPlayerState* OtherPlayerState = Cast<AElvenRingPlayerState>(PlayerState))
	{
		OtherPlayerState->CurrentBossDamage = CurrentBossDamage;
	}
}
