// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/Grux_AnimInstance.h"

#include "ElvenRing/ElvenRing.h"

UGrux_AnimInstance::UGrux_AnimInstance()
{
	IsAttacking = false;
	IsHit = false;
	IsWaiting = false;
	IsDeath = false;
}


void UGrux_AnimInstance::AttackAnim()
{
	if (IsAttacking == false)
	{
		IsAttacking = true;
	}
}

void UGrux_AnimInstance::HitAnim()
{
	if (IsHit == false)
	{
		IsHit = true;
		UE_LOG(LogTemp, Error, TEXT("또 감"));

	}
	else if (IsHit == true)
	{
		IsHit = false;
		IsHit = true;
	}
}

void UGrux_AnimInstance::DeathAnim()
{
	IsDeath = true;
}

// void UGrux_AnimInstance::AnimNotify_ChangeBool()
// {
// 	IsHit = false;
// 	UE_LOG(LogTemp, Error, TEXT("다시 돌아옴"));
// }
