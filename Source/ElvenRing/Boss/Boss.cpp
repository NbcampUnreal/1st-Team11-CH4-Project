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
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/Core/ElvenRingGameMode.h"
#include "ElvenRing/UI/BossWidget.h"
#include "ElvenRing/UI/UIManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#pragma region 생성자, 초기화, Tick 함수
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

	bReplicates = true;
}

void ABoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoss, TargetPlayer);
	DOREPLIFETIME(ABoss, BossState);
	DOREPLIFETIME(ABoss, bIsAttacking);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
	LOG(TEXT("Begin"));

	CurHealth = MaxHealth;

	AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABoss::OnMontageEnded);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
	RegisterCollision(AttackCollision, CollisionSocketName);
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 상태에 해당하는 OnStateUpdate 함수 호출
	UpdateState();
}
#pragma endregion

#pragma region StatePattern 관련 함수
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
#pragma endregion

#pragma region 시퀀스 종료 핸들러 함수 
void ABoss::OnSpawnSequenceEnded()
{
	AttachDelegateToWidget(ECharacterType::Boss);//ksw
	OnRep_HealthChanged();
	LOG(TEXT("Begin"));
}

void ABoss::OnPhaseSequenceEnded()
{
	AttachDelegateToWidget(ECharacterType::Boss);//ksw
	LOG(TEXT("Begin"));
}
#pragma endregion

#pragma region Animation, Sound 재생 함수
void ABoss::PlayAnimation_Implementation(UAnimMontage* MontageToPlay, float PlayRate, FName StartSectionName)
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
}

void ABoss::PlaySound_Implementation(USoundBase* Sound)
{
	AudioComponent->SetSound(Sound);
	AudioComponent->Play();
}
#pragma endregion

#pragma region 공격 판정 함수
void ABoss::OnAttackStarted(TArray<UCapsuleComponent*> Collision)
{
	LOG(TEXT("Begin"));
	bIsAttacking = true;
	for (UCapsuleComponent* Coll : Collision)
	{
		Coll->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ABoss::OnAttackEnded(TArray<UCapsuleComponent*> Collision)
{
	LOG(TEXT("Begin"));
	bIsAttacking = false;
	for (UCapsuleComponent* Coll : Collision)
	{
		Coll->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABoss::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//LOG(TEXT("Mesh Overlap Begin with!: %s"), *OtherActor->GetName());

	if (!bIsAttacking) return;

	AElvenRingCharacter* Player = Cast<AElvenRingCharacter>(OtherActor);
	
	if (Player)
	{
		//LOG(TEXT("Mesh Overlap Begin with Player!"));
		UGameplayStatics::ApplyDamage(Player, AttackPower, GetInstigatorController(), this, UDamageType::StaticClass());
		bIsAttacking = false;	
	}	
	
}

void ABoss::OnMeshOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	LOG(TEXT("Mesh Overlap End: %s"), *OtherActor->GetName());
}
#pragma endregion

#pragma region 보스 유틸 함수
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

void ABoss::SetBossBattleMode_Implementation()
{
	if (!HasAuthority()) return;
	
	LOG(TEXT("Begin"));
	// 공격할 타겟 플레이어 지정
	SetAttackTarget();
	SetAttackTimer();

	// 현재 상태를 Idle로 전환
	CurrentState = IdleState;
	ChangeState(CurrentState);

	// 공격 활성화
	bCanAttack = true;
}

void ABoss::MulticastStopBattleMode_Implementation()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
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

void ABoss::RegisterCollision(UCapsuleComponent* Collision, const FName SocketName)
{
	if (GetMesh()->DoesSocketExist(SocketName))
	{
		Collision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnMeshOverlapBegin);
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABoss::SetAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
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
	AElvenRingGameMode* GameMode = Cast<AElvenRingGameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameMode))
	{
		ACharacter* Character = GameMode->GetHighestDamageCharacter(this);
		LOG(TEXT("Character %s"), *Character->GetName())
		if (IsValid(Character))
		{
			AElvenRingCharacter* Player = Cast<AElvenRingCharacter>(Character);
			LOG(TEXT("Player %s"), *Player->GetName())
			TargetPlayer = Player;
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

void ABoss::ApplyShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeClass, const float CameraShakeScale)
{
	CameraController->ShakeCamera(CameraShakeClass, CameraShakeScale);
}

void ABoss::OnDeath()
{
	Super::OnDeath();

	LOG(TEXT("Begin"));
	Cast<UElvenringGameInstance>(GetGameInstance())->GetUIManager()->GetBossWidgetUi()->SetActiveWidget(false);
	StartDeadSequence();
	Destroy();
}
#pragma endregion