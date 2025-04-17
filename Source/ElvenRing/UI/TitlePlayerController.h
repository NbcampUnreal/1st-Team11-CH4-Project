// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATitlePlayerController();

protected:
	virtual void BeginPlay() override;

};
