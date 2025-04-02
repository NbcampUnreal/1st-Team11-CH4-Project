#include "Boss.h"
#include "BossPattern/BossNormalPatternComponent.h"
#include "BossPattern/BossSpecialPatternComponent.h"
#include "EngineUtils.h"

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
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABoss::ABoss::OnMontageEnded);
	SetAttackTimer();
	SetAttackTargetTimer();
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



void ABoss::PlayAnimMontage(UAnimMontage* MontageToPlay, float PlayRate)
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(MontageToPlay))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(MontageToPlay, PlayRate);
		}
	}
}



void ABoss::SetAttackTimer()
{
	const float RandomAttackTime = FMath::RandRange(AttackInterval, AttackInterval + 0.5f);
	
	GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&ABoss::Attack,
		RandomAttackTime,
		false,
		-1.0f);
}



void ABoss::SetAttackTargetTimer()
{
	const float RandomGetAttackTargetTime = FMath::RandRange(GetAttackTargetInterval - 2.0f, GetAttackTargetInterval + 2.0f);
	
	GetWorldTimerManager().SetTimer(
		GetAttackTargetTimerHandle,
		this,
		&ABoss::SetAttackTarget,
		RandomGetAttackTargetTime,
		false,
		-1.0f);
}



void ABoss::SetAttackTarget()
{
	// 딜량이 높은 플레이어 우선으로 타겟 설정해야함
	// 공격 중에 SetAttackTarget이 호출되어도 이미 이전 타겟을 향해 공격 중이기 때문에
	// 새로운 타겟이 설정되어도 문제가 없을 것으로 보임
	
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		LOG(TEXT("%s"), *PlayerController->GetPawn()->GetName());
	}

	SetAttackTargetTimer();
}



void ABoss::RotateToTarget()
{
	
}