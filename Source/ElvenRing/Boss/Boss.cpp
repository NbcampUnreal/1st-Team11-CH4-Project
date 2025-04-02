#include "Boss.h"
#include "BossPattern/BossNormalPatternComponent.h"
#include "BossPattern/BossSpecialPatternComponent.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	NormalPattern = CreateDefaultSubobject<UBossNormalPatternComponent>("Normal Pattern");
	SpecialPattern = CreateDefaultSubobject<UBossSpecialPatternComponent>("Special Pattern");
}