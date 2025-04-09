// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/Grux_AnimInstance.h"

UGrux_AnimInstance::UGrux_AnimInstance()
{
	IsAttacking = false;
	IsHit = false;
	IsWaiting = false;
	IsDeath = false;
}



void UGrux_AnimInstance::AttackAnim()
{
	IsAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		IsAttacking = false;
		UE_LOG(LogTemp, Warning, TEXT("IsAttacking 초기화 진행"));
	}, 0.8f, false);
}

void UGrux_AnimInstance::HitAnim()
{
	IsHit = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		IsHit = false;
		UE_LOG(LogTemp, Warning, TEXT("IsHit 초기화 진행"));
	}, 0.5f, false);
}

void UGrux_AnimInstance::DeathAnim()
{
	IsDeath = true;
}
