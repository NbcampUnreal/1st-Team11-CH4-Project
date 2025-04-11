// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ElvenRingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AElvenRingPlayerState();

	void RecordPlayerDamage(AActor* DamagedActor, float Damage);
	float GetBossDamage(const AActor* BossActor) const;
	void ResetPlayerDamage(AActor* BossClass);
protected:
	static UClass* GetNativeClass(const AActor* Actor);
	TMap<UClass*, float> BossDamageRecord;

protected:
	virtual void BeginPlay() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
