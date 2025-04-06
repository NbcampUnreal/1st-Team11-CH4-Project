#include "BossSpecialAttackState.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"

void UBossSpecialAttackState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Attacking;
	Boss->SpecialPattern->ExecuteAttackPattern();
}

void UBossSpecialAttackState::OnStateUpdate()
{
	
}

void UBossSpecialAttackState::OnStateExit()
{
	CurrentBoss->SetAttackTimer();
}