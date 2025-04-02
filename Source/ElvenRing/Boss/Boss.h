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

	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
private:
	/** 공격 패턴 타이머 설정하는 함수 */
	virtual void SetAttackTimer();
	
public:

protected:
	/** 보스 일반공격 패턴 컴포넌트 */
	TObjectPtr<class UBossNormalPatternComponent> NormalPattern;

	/** 보스 특수공격 패턴 컴포넌트 */
	TObjectPtr<class UBossSpecialPatternComponent> SpecialPattern;

	/** 공격패턴 간격 */
	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackInterval;

	UAnimInstance* AnimInstance;

private:
	FTimerHandle AttackTimerHandle;
	
};