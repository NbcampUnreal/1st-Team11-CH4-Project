#pragma once

#include "CoreMinimal.h"
#include "BossPatternComponent.h"
#include "BossNormalPatternComponent.generated.h"


UCLASS()
class ELVENRING_API UBossNormalPatternComponent : public UBossPatternComponent
{
	GENERATED_BODY()

public:
	UBossNormalPatternComponent();
	virtual void BeginPlay() override;

	/** 일반공격 패턴 실행 함수 */
	virtual void ExecuteAttackPattern() override;

private:
	void ShuffleAttackOrder();

private:
	TArray<int8> AttackOrder;
	int8 CurrentAttackIndex;
	
};