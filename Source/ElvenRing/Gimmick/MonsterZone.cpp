// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterZone.h"

#include "Components/ShapeComponent.h"
#include "ElvenRing/NormalAI/NormalMonster.h"


// Sets default values
AMonsterZone::AMonsterZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MonsterCount = 0;
}

void AMonsterZone::OnMonsterDeath(AUnitBase* Unit)
{
	MonsterCount--;
	if (MonsterCount == 0)
	{
		OnMonsterZoneClear();
	}
}

// Called when the game starts or when spawned
void AMonsterZone::BeginPlay()
{
	Super::BeginPlay();

	// Collect All Monster in this zone
	TArray<AActor*> OverlappingActors;
	GetCollisionComponent()->GetOverlappingActors(OverlappingActors, ANormalMonster::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (ANormalMonster* Monster = Cast<ANormalMonster>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Monster found: %s"), *Monster->GetName());
			Monster->OnDeathEvent.AddDynamic(this, &AMonsterZone::OnMonsterDeath);
			MonsterCount++;
		}
	}
}

void AMonsterZone::OnMonsterZoneClear() const
{
	OnMonsterZoneClearEvent.Broadcast();
}
