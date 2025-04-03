// Fill out your copyright notice in the Description page of Project Settings.

#include "Navigation/PathFollowingComponent.h"
#include "ElvenRing/NormalAI/NormalAIController.h"

#include "NormalMonster.h"

void ANormalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANormalAIController::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Success BeginPLay"));
}

void ANormalAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	// if (Result.Code == EPathFollowingResult::Success)
	// {
	// 	MoveToCurrentPatrolPoint();
	// }
}

// void ANormalAIController::MoveToCurrentPatrolPoint()
// {
//
// 	ANormalMonster* MyEnemyChar = Cast<ANormalMonster>(GetPawn());
// 	if (!MyEnemyChar)
// 	{
// 		return;
// 	}
//
// 	// 순찰 포인트가 하나도 없다면 이동할 필요 없음
// 	if (MyEnemyChar->PatrolPoints.Num() == 0)
// 	{
// 		return;
// 	}
//     
// 	MoveToActor(
// 		MyEnemyChar->PatrolPoints[CurrentPatrolIndex],
// 		5.0f,   // AcceptanceRadius: 목표 지점 근처 몇 유닛 이내에 도달하면 멈출지
// 		true,   // bStopOnOverlap
// 		true,   // bUsePathfinding
// 		false,  // bCanStrafe: 기본 이동 모드에서 좌우로 회전 없이 이동 가능 여부
// 		nullptr,// FilterClass: 경로 필터. 디폴트 사용
// 		true    // bAllowPartialPath: 부분 경로 허용 여부
// 	);
//
// 	CurrentPatrolIndex = (CurrentPatrolIndex + 1) % MyEnemyChar->PatrolPoints.Num();
// 	
// }