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

	UFUNCTION(BlueprintCallable)
	void SetCanSpawnBoss(bool NewSpawn);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanSpawnBoss = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class ANormalLevelSequenceActor> BossSequenceActor;
};
