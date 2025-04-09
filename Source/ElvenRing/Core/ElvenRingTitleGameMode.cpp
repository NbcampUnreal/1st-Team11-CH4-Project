// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingTitleGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"

AElvenRingTitleGameMode::AElvenRingTitleGameMode()
{
	DefaultPawnClass = nullptr;
}

void AElvenRingTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		EGameInstance->GetUIManager()->ShowTitleScreen(GetWorld());
	}
}
