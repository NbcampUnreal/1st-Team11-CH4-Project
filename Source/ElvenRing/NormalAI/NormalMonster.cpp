// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "Engine/DamageEvents.h"

#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"

ANormalMonster::ANormalMonster()
{
	MaxHealth = 100;
	CurHealth = MaxHealth;
	AttackPower = 10;
	MoveSpeed = 10;
	bCanAttack = true;
	bCanMove = true;
	bIsHit = false;
	bIsDie = false;

	AIControllerClass = ANormalAIController::StaticClass();
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalMonster::Attack(AActor* Target)
{
	Super::Attack(Target);

	AElvenRingCharacter* Character = Cast<AElvenRingCharacter>(Target);
	AController* LocalController = GetController();
	ANormalAIController* AIController = Cast<ANormalAIController>(LocalController);

	FPointDamageEvent DamageEvent;
	if (Character && AIController)
	{
		Character->TakeDamage(AttackPower, DamageEvent, AIController, this);	
	}
	
}
