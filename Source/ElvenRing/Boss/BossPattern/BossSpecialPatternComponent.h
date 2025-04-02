#pragma once

#include "CoreMinimal.h"
#include "BossPatternComponent.h"
#include "BossSpecialPatternComponent.generated.h"

/** 특수공격 패턴용 델리게이트 (조건 확인 함수 포함) */
DECLARE_DELEGATE_RetVal(bool, FSpecialAttackConditionDelegate);


UCLASS()
class ELVENRING_API UBossSpecialPatternComponent : public UBossPatternComponent
{
	GENERATED_BODY()

public:
	UBossSpecialPatternComponent();

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/** 특수공격 패턴 실행 함수 */
	virtual void ExecuteAttackPattern() override;

	/** 특수공격 패턴 추가 함수 */
	void AddAttackPattern(const FSpecialAttackConditionDelegate& InCondition, const FAttackExecuteDelegate& InExecute, const FString& InName);

	/** 실행 가능한 특수공격 함수의 인덱스를 반환하는 함수 */
	int8 GetPossibleSpecialPattern();

protected:
	/** 특수공격 조건 델리게이트 */
	TArray<FSpecialAttackConditionDelegate> PatternConditions;
	
};