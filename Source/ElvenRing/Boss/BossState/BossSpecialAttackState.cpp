#include "BossSpecialAttackState.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"

void UBossSpecialAttackState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Attacking;
	if (IsValid(Boss->TargetPlayer))
	{
		Boss->SpecialPattern->ExecuteAttackPattern();
	}
}

void UBossSpecialAttackState::OnStateUpdate()
{
	
}

void UBossSpecialAttackState::OnStateExit()
{
	CurrentBoss->SetAttackTimer();
}