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
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	
public:
	void HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName) const;
protected:
	/** Client가 Loading Screen을 출력하도록 전달, 현재 Close 하는 것은 Client가 직접하고 있다.*/
	void BroadcastLoadingScreen(const FString& MapName) const;
	
public:
	class UEventManager* GetEventManager() const
	{
		return EventManager;
	}
protected:
	UPROPERTY(BlueprintReadOnly)
	class UEventManager* EventManager;
};
