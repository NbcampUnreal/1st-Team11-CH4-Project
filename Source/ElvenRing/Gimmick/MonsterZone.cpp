// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterZone.h"


// Sets default values
AMonsterZone::AMonsterZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonsterZone::BeginPlay()
{
	Super::BeginPlay();

	// Collect All Monster in this zone
}

// Called every frame
void AMonsterZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

