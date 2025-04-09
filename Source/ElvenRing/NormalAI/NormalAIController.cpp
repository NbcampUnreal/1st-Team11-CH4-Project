// Fill out your copyright notice in the Description page of Project Settings.

#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing/NormalAI/BBNormalAI.h"
#include "ElvenRing/NormalAI/BTNormalAI.h"

ANormalAIController::ANormalAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ANormalAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeAsset && BlackboardComp)
	{
		UseBlackboard(BehaviorTreeAsset->BlackboardAsset, BlackboardComp); // 블랙보드 연결
		RunBehaviorTree(BehaviorTreeAsset); // Behavior Tree 실행
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BehaviorTreeAsset 또는 BlackboardComp가 nullptr입니다!"));
	}
}



void ANormalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}




