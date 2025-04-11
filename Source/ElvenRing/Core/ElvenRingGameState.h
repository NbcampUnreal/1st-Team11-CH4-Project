// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ElvenRingGameState.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingGameState : public AGameState
{
	GENERATED_BODY()

public:
	AElvenRingGameState();
	
	virtual void BeginPlay() override;

	void RecordPlayerDamage(APlayerController* EventInstigator, AActor* DamagedActor, float Damage);
	APlayerController* GetHighestDamagePlayer(const AActor* BossActor) const;
	void ResetDamageRecord(const UClass* BossClass);

protected:
	static UClass* GetNativeClass(const AActor* Actor);
	
	/*
	 * Player Controller는 파괴 될 수 있기 때문에 NetID를 기준으로 작성
	 */
	TUniqueNetIdMap<TMap<UClass*, float>> PlayerBossDamageRecord;
};
