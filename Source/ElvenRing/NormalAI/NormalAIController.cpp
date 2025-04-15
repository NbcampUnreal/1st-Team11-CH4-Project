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
	
}

void ANormalAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}




