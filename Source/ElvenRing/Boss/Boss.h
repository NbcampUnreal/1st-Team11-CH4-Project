#pragma once

#include "CoreMinimal.h"
#include "AUnitBase.h"
#include "Boss.generated.h"

UCLASS()
class ELVENRING_API ABoss : public AAUnitBase
{
	GENERATED_BODY()

public:
	ABoss();

protected:
	
private:
	
public:

protected:
	/** 보스 일반공격 패턴 컴포넌트 */
	TObjectPtr<class UBossNormalPatternComponent> NormalPattern;

	/** 보스 특수공격 패턴 컴포넌트 */
	TObjectPtr<class UBossSpecialPatternComponent> SpecialPattern;

	/** 공격패턴 간격 */
	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float AttackInterval;

private:
	
};