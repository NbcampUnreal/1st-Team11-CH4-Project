// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
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
	virtual void PostSeamlessTravel() override;
	
public:
	/**
	 * 현재 Boss의 Type을 기준으로 기록하고 있다.
	 * Warning : 같은 보스가 Respawn된다거나 여러 개체가 존재하면 잘못된 계산을 할 수 있다.
	 */
	void RecordDamage(AController* EventInstigator, AActor* DamagedActor, float Damage);
	ACharacter* GetHighestDamageCharacter(const AActor* BossActor) const;
	
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
