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
	virtual void SetAttackTimer();

	/** 공격할 대상을 설정하는 함수 */
	void SetAttackTarget();
	
public:

protected:
	TObjectPtr<class UBossNormalPatternComponent> NormalPattern;
	TObjectPtr<class UBossSpecialPatternComponent> SpecialPattern;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackInterval;

	UAnimInstance* AnimInstance;

private:
	FTimerHandle AttackTimerHandle;
	
};