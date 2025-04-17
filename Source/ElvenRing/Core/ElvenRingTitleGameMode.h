// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ElvenRingTitleGameMode.generated.h"

class ATitleMapParticle;
UCLASS()
class ELVENRING_API AElvenRingTitleGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AElvenRingTitleGameMode();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<ATitleMapParticle> TitleMapParticleClass;
	ATitleMapParticle* TitleMapParticle;
};
