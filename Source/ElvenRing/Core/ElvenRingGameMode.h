// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/ElvenRingController.h"
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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPlayersReady);
	/** 모든 Client에서 Begin Play가 호출된 이후에 호출된다. */
	UPROPERTY(BlueprintAssignable)
	FOnAllPlayersReady OnAllPlayersReadyDelegate;

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
	// Match Start가 될 때 호출, Client들이 Begin Play가 시작된다.
	virtual void HandleMatchHasStarted() override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
public:
	/**
	 * 현재 Boss의 Type을 기준으로 기록하고 있다.
	 * Warning : 같은 보스가 Respawn된다거나 여러 개체가 존재하면 잘못된 계산을 할 수 있다.
	 */
	void RecordDamage(AController* EventInstigator, AActor* DamagedActor, float Damage);
	/** Boss에게 가장 높은 데미지를 입힌 Chracter를 반환한다. 만약, 수치가 같다면 처음 순회한 객체를 반환한다. 캐릭터가 존재하지 않을 경우 nulltpr를 반환한다.*/
	ACharacter* GetHighestDamageCharacter(const AActor* BossActor) const;
	void RecordInvincible(AController* EventInstigator, AUnitBase* DamagedActor, float Damage);
	
public:
	/** Server Travel을 통해서 맵을 이동하게 하고 Client가 로딩 스크린을 출력하도록 한다. */
	void HandleLevelTransition(APlayerController* PlayerController, const FString& LevelName);
	/** 모든 유저가 맵을 로딩했는지 확인 */
	bool AreAllPlayersReady() const;
	/** Player Controller Begin Play에서 호출*/
	void HandleNetworkReady(AElvenRingController* ElvenRingController);

protected:
	/** Client가 Loading Screen을 출력하도록 전달, 현재 Close 하는 것은 Client가 OnPostLoadMap에서 직접하고 있다.*/
	void BroadcastLoadingScreen(const FString& MapName, float FadeOutTime) const;
	/** 모든 Client가 맵을 로딩했을 떄 호출, Start Match로 게임이 진행 된다. */
	void OnAllPlayerLoadMap();
	/** 모든 Client가 BeginPlay가 호출된 다음에 호출 */
	void OnAllPlayerReady();
	
	bool bAfterSeamlessTravel;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAllPlayerLoadMap;
	FTimerHandle LoadingTimeOutHandle;
	UPROPERTY(EditDefaultsOnly)
	float LoadingTimeOutTime;
	/** Player의 Map Load만을 체크해서는 불완전하기에 Begin Play 시점도 확인해야 한다. */
	int32 PlayerReadyCount;
	/** Player Ready 호출 기록 */
	bool bHasPlayersReady;
	UPROPERTY(EditDefaultsOnly)
	float TravelDelayTime;
	FTimerHandle TravelDelayHandle;
public:
	class UEventManager* GetEventManager() const
	{
		return EventManager;
	}
protected:
	UPROPERTY(BlueprintReadOnly)
	class UEventManager* EventManager;
};
