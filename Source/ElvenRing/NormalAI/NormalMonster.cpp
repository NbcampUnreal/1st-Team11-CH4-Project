// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"

ANormalMonster::ANormalMonster()
{
	// 이 캐릭터를 컨트롤할 AIController 클래스를 EnemyAIController로 지정
	AIControllerClass = ANormalAIController::StaticClass();
	// 레벨에 배치되거나 스폰되면 자동으로 AIController가 할당되도록 설정
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

