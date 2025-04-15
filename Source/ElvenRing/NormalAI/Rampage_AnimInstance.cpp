#include "ElvenRing/NormalAI/Rampage_AnimInstance.h"
#include "ElvenRing/NormalAI/RampageMonster.h"

URampage_AnimInstance::URampage_AnimInstance()
{
	IsAttacking = false;
	IsHit = false;
	IsWaiting = false;
	IsDeath = false;
}

void URampage_AnimInstance::UpdateAttack(bool value)
{
	IsAttacking = value;
}

void URampage_AnimInstance::UpdateHit(bool value)
{
	IsHit = value;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() 
	{
		IsHit = false;
		AActor* OwnerActor = GetOwningActor();
	}, 0.1f, false);	
}

void URampage_AnimInstance::UpdateDeath(bool value)
{
	IsDeath = value;
}

void URampage_AnimInstance::AnimNotify_EndHit()
{IsHit=false;
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ARampageMonster* Monster = Cast<ARampageMonster>(OwnerActor);
		if (Monster)
		{	
			Monster->RPCIsHit(IsHit,OwnerActor);
		}
	}
}

void URampage_AnimInstance::AnimNotify_EndAttack()
{
	AActor* OwnerActor = GetOwningActor();
	if (OwnerActor)
	{
		ARampageMonster* Monster = Cast<ARampageMonster>(OwnerActor);
		if (Monster)
		{
			IsAttacking=false;
			Monster->MulticastIsAttack(IsAttacking);
		}
	}
}
