// Fill out your copyright notice in the Description page of Project Settings.


#include "BossZone.h"

#include "Components/ShapeComponent.h"
#include "ElvenRing/LevelSequence/NormalLevelSequenceActor.h"


// Sets default values
ABossZone::ABossZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bCanSpawnBoss = false;
	bIsBossSpawned = false;
}

void ABossZone::SetCanSpawnBoss(const bool NewSpawn)
{
	bCanSpawnBoss = NewSpawn;

	if (bCanSpawnBoss)
	{
		GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABossZone::ResetBossSpawn()
{
	SetCanSpawnBoss(true);
	bIsBossSpawned = false;
}

void ABossZone::SpawnBoss()
{
	BossSequenceActor->StartSequence();
	bIsBossSpawned = true;
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABossZone::BeginPlay()
{
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABossZone::OnOverlapBegin);
}

void ABossZone::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCanSpawnBoss)
	{
		if (BossSequenceActor)
		{
			SpawnBoss();
		}
	}
}
