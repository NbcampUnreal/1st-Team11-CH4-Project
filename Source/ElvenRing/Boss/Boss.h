#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Boss.generated.h"


class IBossStateInterface;
class AElvenRingCharacter;

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle        UMETA(DisplayName = "Idle"),
	Moving     UMETA(DisplayName = "Moving"),
	Attacking  UMETA(DisplayName = "Attacking"),
};

UCLASS()
class ELVENRING_API ABoss : public AUnitBase
{
	GENERATED_BODY()

public:
	ABoss();

	void ChangeState(IBossStateInterface* State);

	/** 타겟과의 거리를 구하는 함수 */
	float GetDistanceBetweenTarget() const;

	/** 타겟을 향한 방향벡터를 구하는 함수 */
	UFUNCTION(BlueprintCallable)
	FVector GetDirectionVectorToTarget() const;

	/** 컷신 종료 후 호출할 함수로, 보스가 전투를 시작하는 상태가 됨 */
	UFUNCTION(BlueprintCallable)
	void SetBossBattleMode();

	/** 특정 구간동안 빠르게 이동시키고 싶을 때 사용 */
	UFUNCTION(BlueprintCallable)
	void MoveForward(float MoveMultiflier);

	/** 공격이 끝났을 때 호출하여 보스 상태 및 이동속도 초기화 */
	UFUNCTION(BlueprintCallable)
	void InitMoveSpeed();

	/** 공격 패턴 타이머 설정하는 함수 */
	void SetAttackTimer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float PlayAnimMontage(UAnimMontage* MontageToPlay, float PlayRate = 1.0f, FName StartSectionName = NAME_None) override;
	
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	/** 공격할 대상 탐색 주기 설정 함수 */
	void SetAttackTargetTimer();

	/** 공격할 대상을 설정하는 함수 */
	void SetAttackTarget();

	/** 공격할 대상을 향해 회전하는 함수 */
	void RotateToTarget(float DeltaTime);
	
	/** 보스의 상태에 맞는 OnStateUpdate 호출하는 함수 */
	void UpdateState();

	/** 공격이 가능한 상태인지 확인 */
	void ChangeToAttackStateIfConditionSatisfied();

	
public:
	TObjectPtr<class UBossNormalPatternComponent> NormalPattern;
	TObjectPtr<class UBossSpecialPatternComponent> SpecialPattern;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|BGM")
	USoundBase* BossBattleBGM;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinAttackRadius;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinMoveRadius;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinIdleRadius;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackInterval;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackIntervalRange;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float GetAttackTargetInterval;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Boss|Anim")
	UAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Target")
	AElvenRingCharacter* TargetPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Target")
	EBossState BossState;

	IBossStateInterface* CurrentState, *IdleState, *MoveState, *AttackState, *SpecialAttackState;

private:
	FTimerHandle AttackTimerHandle;
	FTimerHandle GetAttackTargetTimerHandle;
	FTimerHandle AnimationMontageHandle;

	UPROPERTY()
	UAudioComponent* AudioComponent;
	
};