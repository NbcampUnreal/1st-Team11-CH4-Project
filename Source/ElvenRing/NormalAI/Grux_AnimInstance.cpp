// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/Grux_AnimInstance.h"
#include "ElvenRing/NormalAI/NormalMonster.h"

#include "ElvenRing/ElvenRing.h"

UGrux_AnimInstance::UGrux_AnimInstance()
{
	IsAttacking = false;
	IsHit = false;
	IsWaiting = false;
	IsDeath = false;
}


void UGrux_AnimInstance::UpdateAttack(bool value)
{
	IsAttacking = value;
}

void UGrux_AnimInstance::UpdateHit(bool value)
{
	IsHit = value;
	UE_LOG(LogTemp, Warning, TEXT("HitUpdate 호출"));
}

void UGrux_AnimInstance::UpdateDeath(bool value)
{
	IsDeath = value;
}

void UGrux_AnimInstance::AnimNotify_EndHit()
{
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ANormalMonster* Monster = Cast<ANormalMonster>(OwnerActor);
		if (Monster)
		{	IsHit=false;
			Monster->MulticastIsHit(IsHit);
		}
	}
}

void UGrux_AnimInstance::AnimNotify_EndAttack()
{
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ANormalMonster* Monster = Cast<ANormalMonster>(OwnerActor);
		if (Monster)
		{
			IsAttacking=false;
			Monster->MulticastIsAttack(IsAttacking);
		}
	}
}
