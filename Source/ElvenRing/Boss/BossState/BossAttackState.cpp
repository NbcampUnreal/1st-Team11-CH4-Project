#include "BossAttackState.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"

void UBossAttackState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Attacking;
	if (IsValid(Boss->TargetPlayer))
	{
		Boss->NormalPattern->ExecuteAttackPattern();
	}
}

void UBossAttackState::OnStateUpdate()
{
	
}

void UBossAttackState::OnStateExit()
{
	CurrentBoss->SetAttackTimer();
}