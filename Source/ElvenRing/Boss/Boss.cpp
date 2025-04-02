#include "Boss.h"
#include "BossPattern/BossNormalPatternComponent.h"
#include "BossPattern/BossSpecialPatternComponent.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	NormalPattern = CreateDefaultSubobject<UBossNormalPatternComponent>("Normal Pattern");
	SpecialPattern = CreateDefaultSubobject<UBossSpecialPatternComponent>("Special Pattern");
}



void ABoss::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	SetAttackTimer();
}



void ABoss::Attack()
{
	Super::Attack();

	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	NormalPattern->ExecuteAttackPattern();
}



void ABoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	SetAttackTimer();
}



void ABoss::SetAttackTimer()
{
	const float RandomAttackTime = FMath::RandRange(AttackInterval, AttackInterval+0.5f);
	
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABoss::Attack,
		RandomAttackTime,
		false,
		-1.0f);
}
