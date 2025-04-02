#include "BossIdleState.h"
#include "ElvenRing/Boss/Boss.h"

void UABossIdleState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Idle;
}

void UABossIdleState::OnStateUpdate()
{
	// 1. 플레이어와 거리가 500 이상인 경우 Move 상태로 전이
	if (CurrentBoss->GetDistanceBetweenTarget() >= CurrentBoss->MinMoveRadius)
	{
		CurrentBoss->ChangeState(CurrentBoss->MoveState);
	}
}

void UABossIdleState::OnStateExit()
{
	
}
