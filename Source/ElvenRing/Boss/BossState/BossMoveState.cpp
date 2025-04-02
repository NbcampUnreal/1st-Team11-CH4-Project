#include "BossMoveState.h"
#include "ElvenRing/Boss/Boss.h"

void UABossMoveState::OnStateEnter(ABoss* Boss)
{
	CurrentBoss = Boss;
	CurrentBoss->BossState = EBossState::Moving;
}

void UABossMoveState::OnStateUpdate()
{
	// 1. 플레이어를 향한 방향 벡터 반환
	const FVector Direction = CurrentBoss->GetDirectionVectorToTarget();

	// 2. 플레이어를 향해 이동
	CurrentBoss->AddMovementInput(Direction);
	
	// 3. 플레이어와 거리가 300 미만인 경우 Idle 상태로 전이
	if (CurrentBoss->GetDistanceBetweenTarget() < CurrentBoss->MinIdleRadius)
	{
		CurrentBoss->ChangeState(CurrentBoss->IdleState);
	}
}

void UABossMoveState::OnStateExit()
{
	
}