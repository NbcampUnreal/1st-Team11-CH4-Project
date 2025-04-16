// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ElvenRingResultStatGameMode.generated.h"

class UScorePageWidget;
UCLASS()
class ELVENRING_API AElvenRingResultStatGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AElvenRingResultStatGameMode();
	void BeginPlay();
};
