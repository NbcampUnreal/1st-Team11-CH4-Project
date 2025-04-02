#include "BossPatternComponent.h"

UBossPatternComponent::UBossPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PatternCount = 0;
}