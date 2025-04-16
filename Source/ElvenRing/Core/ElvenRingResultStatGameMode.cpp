// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingResultStatGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"


AElvenRingResultStatGameMode::AElvenRingResultStatGameMode()
{
	DefaultPawnClass = nullptr;
}
void AElvenRingResultStatGameMode::BeginPlay()
{
	Super::BeginPlay();

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		EGameInstance->GetUIManager()->ShowScorePageWidget(GetWorld());
	}
}