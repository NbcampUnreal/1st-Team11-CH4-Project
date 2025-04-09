// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "BossZone.generated.h"

UCLASS()
class ELVENRING_API ABossZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	/** Spawn 가능 여부를 설정, true일지라도 bIsSpawned가 true이면 다시 Spawn되지 않는다.*/
	UFUNCTION(BlueprintCallable)
	void SetCanSpawnBoss(bool NewSpawn);

	UFUNCTION(BlueprintCallable)
	void ResetBossSpawn();
protected:
	void SpawnBoss();

	/** Boss가 Spawn될 수 있는지 여부*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanSpawnBoss;
	/** Spawn할 Boss의 Sequence */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class ANormalLevelSequenceActor> BossSequenceActor;

private:
	/** Boss가 Spawn되었는지 여부*/
	bool bIsBossSpawned;
};
