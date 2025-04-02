#include "BossAttackState.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"

void UBossAttackState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Attacking;
	Boss->NormalPattern->ExecuteAttackPattern();
}

void UBossAttackState::OnStateUpdate()
{
	
}

void UBossAttackState::OnStateExit()
{
	CurrentBoss->SetAttackTimer();
}