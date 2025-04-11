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
}

void UGrux_AnimInstance::AnimNotify_EndHit()
{
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ANormalMonster* Monster = Cast<ANormalMonster>(OwnerActor);
		if (Monster)
		{
			Monster->InstanceIsHit = false;
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
			Monster->InstanceIsAttack = false;
		}
	}
}
