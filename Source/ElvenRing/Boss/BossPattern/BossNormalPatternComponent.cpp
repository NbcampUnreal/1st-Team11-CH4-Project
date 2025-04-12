#include "BossNormalPatternComponent.h"
#include "ElvenRing/ElvenRing.h"

UBossNormalPatternComponent::UBossNormalPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PatternCount = 0;
	CurrentAttackIndex = 0;
	AttackOrder.Empty();
}

void UBossNormalPatternComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int8 i=0; i<PatternCount; ++i)
	{
		AttackOrder.Add(i);
	}
}

void UBossNormalPatternComponent::ExecuteAttackPattern()
{
	// 추가한 패턴이 없다면 리턴
	if (PatternCount <= 0)
	{
		LOG(TEXT("Pattern No Exist"));
		return;
	}
	
	const int8 Index = AttackOrder[CurrentAttackIndex];
	
	if (Patterns.IsValidIndex(Index))
	{
		Patterns[Index].Execute();	
	}

	if (CurrentAttackIndex < PatternCount-1)
	{
		++CurrentAttackIndex;
	}
	else
	{
		CurrentAttackIndex = 0;
		ShuffleAttackOrder();
	}
}

void UBossNormalPatternComponent::ShuffleAttackOrder()
{
	for (int8 i=0; i<PatternCount; ++i)
	{
		const int8 RandNum = FMath::RandRange(0, PatternCount - 1);
		if (i != RandNum)
		{
			AttackOrder.Swap(i, RandNum);
		}
	}
}
