#include "BossNormalPatternComponent.h"
#include "ElvenRing/ElvenRing.h"

UBossNormalPatternComponent::UBossNormalPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PatternCount = 0;
}

void UBossNormalPatternComponent::ExecuteAttackPattern()
{
	// 추가한 패턴이 없다면 리턴
	if (PatternCount <= 0)
	{
		LOG(TEXT("Pattern No Exist"));
		return;
	}

	// 보유한 패턴 중 랜덤으로 1개 실행
	const int8 RandNum = FMath::RandRange(0, PatternCount - 1);
	if (Patterns.IsValidIndex(RandNum))
	{
		Patterns[RandNum].Execute();	
	}
}
