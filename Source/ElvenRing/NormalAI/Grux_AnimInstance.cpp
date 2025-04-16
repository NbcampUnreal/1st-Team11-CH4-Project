
#include "ElvenRing/NormalAI/Grux_AnimInstance.h"
#include "ElvenRing/NormalAI/NormalMonster.h"

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
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
	// 	{
	// 		IsHit = false;
	// 		AActor* OwnerActor = GetOwningActor();
	// 	}, 0.8f, false);	
}

void UGrux_AnimInstance::UpdateDeath(bool value)
{
	IsDeath = value;
}

void UGrux_AnimInstance::AnimNotify_EndHit()
{
	IsHit=false;
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ANormalMonster* Monster = Cast<ANormalMonster>(OwnerActor);
		if (Monster)
		{	
			Monster->RPCIsHit(IsHit,OwnerActor);
		}
	}
}

void UGrux_AnimInstance::AnimNotify_EndAttack()
{
	IsAttacking=false;
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ANormalMonster* Monster = Cast<ANormalMonster>(OwnerActor);
		if (Monster)
		{
			Monster->MulticastIsAttack(IsAttacking);
		}
	}
}