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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartMatch);
	UPROPERTY(BlueprintAssignable)
	FOnStartMatch OnStartMatchDelegate;
	
protected:
	// Call 순서
	// StartToLeaveMap(Seamless Travel 직전)
	// PostSeamlessTravel
	// BeginPlay
	// StartPlay
	/** Ready To Start Match가 True가 되면 Start Match가 진행된다. Tick에서 매 프레임 체크된다. */
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void StartPlay() override;
	virtual void PostSeamlessTravel() override;
	virtual void StartToLeaveMap() override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;
	// Match Start가 될 때 호출
	virtual void HandleMatchHasStarted() override;
	
public:
	/**
	 * 현재 Boss의 Type을 기준으로 기록하고 있다.
	 * Warning : 같은 보스가 Respawn된다거나 여러 개체가 존재하면 잘못된 계산을 할 수 있다.
	 */
	void RecordDamage(AController* EventInstigator, AActor* DamagedActor, float Damage);
	ACharacter* GetHighestDamageCharacter(const AActor* BossActor) const;
	
public:
	/** Server Travel을 통해서 맵을 이동하게 하고 Client가 로딩 스크린을 출력하도록 한다. */
	void HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName) const;
	/** 모든 유저가 맵을 로딩했는지 확인 */
	bool AreAllPlayersReady() const;
	DECLARE_MULTICAST_DELEGATE(FOnAllPlayersReady);
	FOnAllPlayersReady OnAllPlayersReadyDelegate;

protected:
	/** Client가 Loading Screen을 출력하도록 전달, 현재 Close 하는 것은 Client가 OnPostLoadMap에서 직접하고 있다.*/
	void BroadcastLoadingScreen(const FString& MapName) const;
	/** 모든 Client가 맵을 로딩했을 떄 호출 */
	void OnAllPlayersReady();
	
	bool bAfterSeamlessTravel;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAllPlayersReady;
	FTimerHandle LoadingTimeOutHandle;
	UPROPERTY(EditDefaultsOnly)
	float LoadingTimeOutTime;
public:
	class UEventManager* GetEventManager() const
	{
		return EventManager;
	}
protected:
	UPROPERTY(BlueprintReadOnly)
	class UEventManager* EventManager;
};
