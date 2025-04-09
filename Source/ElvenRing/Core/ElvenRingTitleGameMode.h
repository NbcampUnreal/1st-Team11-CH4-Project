// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ElvenRingTitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingTitleGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AElvenRingTitleGameMode();
protected:
	virtual void BeginPlay() override;

};
