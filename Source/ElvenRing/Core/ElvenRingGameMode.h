// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ElvenRingGameMode.generated.h"

/*
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AElvenRingGameMode();
	void HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName);
};
