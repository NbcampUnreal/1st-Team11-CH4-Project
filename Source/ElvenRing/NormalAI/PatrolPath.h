// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class ELVENRING_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	// 레벨에 배치된 웨이포인트 목록
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Patrol")
	TArray<AActor*> Waypoints;

	// 인덱스로 웨이포인트를 얻는 함수
	AActor* GetWaypoint(int32 Index) const;

	// 웨이포인트 개수 반환
	int32 Num() const;
};
