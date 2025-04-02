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

	/** 일반공격 패턴 실행 함수 */
	virtual void ExecuteAttackPattern() override;
	
};