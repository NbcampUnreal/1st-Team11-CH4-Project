#include "BossSpecialPatternComponent.h"
#include "ElvenRing/ElvenRing.h"

UBossSpecialPatternComponent::UBossSpecialPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PatternCount = 0;
}

void UBossSpecialPatternComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ExecuteAttackPattern();
}

void UBossSpecialPatternComponent::ExecuteAttackPattern()
{
	// 추가한 패턴이 없다면 리턴
	if (PatternCount <= 0)
	{
		return;
	}

	// 실행 가능한 특수공격 패턴의 인덱스를 가져와 실행
	int8 PossibleSpeicalPatternIndex = GetPossibleSpecialPattern();
	if (Patterns.IsValidIndex(PossibleSpeicalPatternIndex))
	{
		Patterns[PossibleSpeicalPatternIndex].Execute();
	}
}



void UBossSpecialPatternComponent::AddAttackPattern(const FSpecialAttackConditionDelegate& InCondition,
                                                    const FAttackExecuteDelegate& InExecute, const FString& InName)
{
	PatternConditions[PatternCount] = InCondition;
	Patterns[PatternCount] = InExecute;
	PatternName[PatternCount] = InName;
	++PatternCount;
}



int8 UBossSpecialPatternComponent::GetPossibleSpecialPattern()
{
	for (int8 i = 0; i < PatternCount; ++i)
	{
		if (PatternConditions[i].IsBound() && PatternConditions[i].Execute())
		{
			LOG(TEXT("Pattern %d is Available"), i);
			return i;
		}
	}
	
	return -1;
}