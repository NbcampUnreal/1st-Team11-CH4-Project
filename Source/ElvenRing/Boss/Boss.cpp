#include "Boss.h"
#include "BossPattern/BossNormalPatternComponent.h"
#include "BossPattern/BossSpecialPatternComponent.h"
#include "EngineUtils.h"
#include "BossState/BossAttackState.h"
#include "BossState/BossIdleState.h"
#include "BossState/BossMoveState.h"
#include "BossState/BossSpecialAttackState.h"
#include "BossState/BossStateInterface.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Effect/CameraControllerComponent.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// 보스 패턴 및 카메라 컴포넌트
	NormalPattern = CreateDefaultSubobject<UBossNormalPatternComponent>("Normal Pattern");
	SpecialPattern = CreateDefaultSubobject<UBossSpecialPatternComponent>("Special Pattern");
	CameraController = CreateDefaultSubobject<UCameraControllerComponent>("Camera Controller");

	// 보스 상태
	IdleState = CreateDefaultSubobject<UABossIdleState>("Idle State");
	MoveState = CreateDefaultSubobject<UABossMoveState>("Move State");
	AttackState = CreateDefaultSubobject<UBossAttackState>("Attack State");
	SpecialAttackState = CreateDefaultSubobject<UBossSpecialAttackState>("Special Attack State");

	// 보스 테마 전용 오디오 컴포넌트
	AudioComponent = CreateDefaultSubobject<UAudioComponent>("BGM");

	// 보스 공격 판정 콜리전
	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>("Attack Collision");
	AttackCollision->SetupAttachment(GetMesh());

	// 기타 포인터
	AnimInstance = nullptr;
	TargetPlayer = nullptr;

	// 보스 스탯
	MinMoveRadius = 500.0f;
	MinAttackRadius = 500.0f;
	MinIdleRadius = 300.0f;
	bIsAttacking = false;
}



void ABoss::BeginPlay()
{
	Super::BeginPlay();

	CurHealth = MaxHealth;

	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABoss::OnMontageEnded);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;

	if (GetMesh()->DoesSocketExist(CollisionSocketName))
	{
		AttackCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CollisionSocketName);
		AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnMeshOverlapBegin);
		AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 상태에 해당하는 OnStateUpdate 함수 호출
	UpdateState();
}

void ABoss::OnDeath()
{
	Super::OnDeath();

	LOG(TEXT("Begin"));
	Destroy();

	// 컷신 재생 로직 구현 필요
}


void ABoss::UpdateState()
{
	if (CurrentState != nullptr)
	{
		CurrentState->OnStateUpdate();
	}
}



void ABoss::ChangeState(IBossStateInterface* State)
{
	if (CurrentState != nullptr)
	{
		CurrentState->OnStateExit();
	}
	CurrentState = State;
	CurrentState->OnStateEnter(this);
}



float ABoss::GetDistanceBetweenTarget() const
{
	if (!IsValid(TargetPlayer)) return 0;
	return FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
}

FVector ABoss::GetDirectionVectorToTarget() const
{
	if (!IsValid(TargetPlayer)) return FVector::ZeroVector;

	FVector Direction = TargetPlayer->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	Direction.Normalize();

	return Direction;
}

void ABoss::SetBossBattleMode()
{
	// 1. 공격할 타겟 플레이어 지정
	SetAttackTarget();
	SetAttackTimer();

	// 2. 현재 상태를 Idle로 전환
	CurrentState = IdleState;
	ChangeState(CurrentState);

	// 3. 공격 활성화
	bCanAttack = true;

	// 4. 보스 BGM 활성화
	AudioComponent->SetSound(BossBattleBGM);
	AudioComponent->Play();
}

void ABoss::MoveForward(const float MoveMultiflier)
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed * MoveMultiflier;
	AddMovementInput(GetActorForwardVector());
}

void ABoss::InitMoveSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}


void ABoss::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bCanAttack = true;
	ChangeState(IdleState);
}



float ABoss::PlayAnimMontage(UAnimMontage* MontageToPlay, float PlayRate, FName StartSectionName)
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	
	if (IsValid(AnimInstance))
	{
		if (IsValid(MontageToPlay))
		{
			AnimInstance->Montage_Play(MontageToPlay, PlayRate);

			// 애니메이션 길이를 가져와서 25% 시점 계산
			const float MontageDuration = MontageToPlay->GetPlayLength() / PlayRate;
			const float DelayTime = MontageDuration * 0.25f;

			// 일정 시간이 지난 후 bCanAttack을 false로 설정하는 타이머 실행
			GetWorldTimerManager().SetTimer(AnimationMontageHandle, FTimerDelegate::CreateLambda([&]
				{
					bCanAttack = false;
				}
				),DelayTime,false
			);
		}
	}

	return 0.f;
}

void ABoss::OnAttackStarted()
{
	LOG(TEXT("Begin"));
	bIsAttacking = true;
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABoss::OnAttackEnded()
{
	LOG(TEXT("Begin"));
	bIsAttacking = false;
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABoss::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LOG(TEXT("Mesh Overlap Begin with!: %s"), *OtherActor->GetName());

	if (!bIsAttacking) return;

	AElvenRingCharacter* Player = Cast<AElvenRingCharacter>(OtherActor);
	
	if (Player)
	{
		LOG(TEXT("Mesh Overlap Begin with Player!"));
		UGameplayStatics::ApplyDamage(Player, AttackPower, GetInstigatorController(), this, UDamageType::StaticClass());
		bIsAttacking = false;	
	}	
	
}

void ABoss::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	LOG(TEXT("Mesh Overlap End: %s"), *OtherActor->GetName());

}


void ABoss::SetAttackTimer()
{
	const float RandomAttackTime = FMath::RandRange(AttackInterval, AttackInterval + AttackIntervalRange);
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ABoss::ChangeToAttackStateIfConditionSatisfied, RandomAttackTime,false);
}



void ABoss::SetAttackTargetTimer()
{
	const float RandomGetAttackTargetTime = FMath::RandRange(GetAttackTargetInterval - 2.0f, GetAttackTargetInterval + 2.0f);
	GetWorldTimerManager().SetTimer(GetAttackTargetTimerHandle, this, &ABoss::SetAttackTarget,RandomGetAttackTargetTime,false);
}



void ABoss::SetAttackTarget()
{
	// 딜량이 높은 플레이어 우선으로 타겟 설정해야함
	// 공격 중에 SetAttackTarget이 호출되어도 이미 이전 타겟을 향해 공격 중이기 때문에
	// 새로운 타겟이 설정되어도 문제가 없을 것으로 보임
	
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
			if (IsValid(Player)) // NewTarget이 유효한지 체크
			{
				//LOG(TEXT("Target set to: %s"), *Player->GetName());
				TargetPlayer = Player;
				break;
			}
		
	}

	SetAttackTargetTimer();
}



void ABoss::RotateToTarget(float DeltaTime)
{
	// 공격 중일 경우 종료
	if (!bCanAttack) return;

	// 타겟이 존재하지 않을 경우 종료
	if (!IsValid(TargetPlayer)) return;

	// 높이를 제외한 방향 벡터 계산
	const FVector Direction = GetDirectionVectorToTarget();

	// 회전 값이 너무 작을 경우 종료
	if (Direction.SizeSquared() < KINDA_SMALL_NUMBER) return;

	const FRotator CurrentRotation = GetActorRotation();
	const FRotator TargetRotation = FRotator(0.0f, Direction.Rotation().Yaw, 0.0f);
	const FRotator InterpRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);

	SetActorRotation(InterpRotation);
}


void ABoss::ChangeToAttackStateIfConditionSatisfied()
{
	if (SpecialPattern->IsSpecialPatternAvailable())
	{
		ChangeState(SpecialAttackState);
	}
	else if (GetDistanceBetweenTarget() <= MinAttackRadius)
	{
		ChangeState(AttackState);	
	}
	else
	{
		SetAttackTimer();
	}
}


void ABoss::ApplyShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeClass, const float CameraShakeScale)
{
	CameraController->ShakeCamera(CameraShakeClass, CameraShakeScale);
}
