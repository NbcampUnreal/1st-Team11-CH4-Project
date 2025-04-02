#include "BossSpecialPatternComponent.h"
#include "ElvenRing/ElvenRing.h"

UBossSpecialPatternComponent::UBossSpecialPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PatternCount = 0;
	PossibleSpecialPatternIndex = 0;
}



void UBossSpecialPatternComponent::ExecuteAttackPattern()
{
	// 추가한 패턴이 없다면 리턴
	if (PatternCount <= 0)
	{
		return;
	}

	// 실행 가능한 특수공격 패턴의 인덱스를 가져와 실행
	if (Patterns.IsValidIndex(PossibleSpecialPatternIndex))
	{
		Patterns[PossibleSpecialPatternIndex].Execute();
	}
}



bool UBossSpecialPatternComponent::IsSpecialPatternAvailable()
{
	for (int8 i = 0; i < PatternCount; ++i)
	{
		if (PatternConditions[i].IsBound() && PatternConditions[i].Execute())
		{
			PossibleSpecialPatternIndex = i;
			return true;
		}
	}

	return false;
}