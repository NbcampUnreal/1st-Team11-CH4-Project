#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Boss.generated.h"

UCLASS()
class ELVENRING_API ABoss : public AUnitBase
{
	GENERATED_BODY()

public:
	ABoss();

protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;

	void PlayAnimMontage(UAnimMontage* MontageToPlay, float PlayRate = 1.0f);
	
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	/** 공격 패턴 타이머 설정하는 함수 */
	void SetAttackTimer();

	/** 공격할 대상 탐색 주기 설정 함수 */
	void SetAttackTargetTimer();

	/** 공격할 대상을 설정하는 함수 */
	void SetAttackTarget();

	/** 공격할 대상을 향해 회전하는 함수 */
	void RotateToTarget();
	
public:

protected:
	TObjectPtr<class UBossNormalPatternComponent> NormalPattern;
	TObjectPtr<class UBossSpecialPatternComponent> SpecialPattern;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackInterval;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float GetAttackTargetInterval;

	UAnimInstance* AnimInstance;

private:
	FTimerHandle AttackTimerHandle;
	FTimerHandle GetAttackTargetTimerHandle;
	
};