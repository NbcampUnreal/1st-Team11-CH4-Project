// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameState.h"

#include "ElvenRing/Character/ElvenRingController.h"

AElvenRingGameState::AElvenRingGameState()
{
}

void AElvenRingGameState::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	// Player Controller가 Seamless Travel을 할 경우 Begin Play가 호출되지 않는다.
	// Game State는 Begin Play를 호출하는 처음 부분이므로 여기서 Player Controller의 ServerReportPlayerReady를 호출한다.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (AElvenRingController* PlayerController = Cast<AElvenRingController>(Iterator->Get()))
		{
			PlayerController->ServerReportPlayerReady();
		}
	}
}