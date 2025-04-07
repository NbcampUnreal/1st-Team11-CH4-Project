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
	UE_LOG(LogTemp, Display, TEXT("Monster %s is dead, Unit Remain : %d"), *Unit->GetName(), MonsterCount);
	if (MonsterCount == 0)
	{
		OnMonsterZoneClear();
	}
}

void AMonsterZone::CountMonsters()
{
	TArray<AActor*> OverlappingActors;
	GetCollisionComponent()->GetOverlappingActors(OverlappingActors, ANormalMonster::StaticClass());
	for (AActor* Actor : OverlappingActors)
	{
		if (ANormalMonster* Monster = Cast<ANormalMonster>(Actor))
		{
			Monster->OnDeathEvent.AddDynamic(this, &AMonsterZone::OnMonsterDeath);
			MonsterCount++;
		}
	}

	UE_LOG(LogTemp, Display, TEXT("Monster Count: %d"), MonsterCount);
}

// Called when the game starts or when spawned
void AMonsterZone::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}
	
	// Begin Play 시점에서는 초기화가 다 끝나지 않아서 Overlap 이벤트가 제대로 동작하지 않는다.
	// 일단은 다음 프레임에서 검출하기로 한다.
	GetWorldTimerManager().SetTimerForNextTick(this, &AMonsterZone::CountMonsters);
}

void AMonsterZone::OnMonsterZoneClear() const
{
	OnMonsterZoneClearEvent.Broadcast();
}
