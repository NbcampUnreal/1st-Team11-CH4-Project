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

	/** 특수공격이 실행 가능한지 여부를 반환하는 함수 */
	bool IsSpecialPatternAvailable();

	/** 특수공격 패턴 실행 함수 */
	virtual void ExecuteAttackPattern() override;
	
	/** 특수공격 패턴 추가 함수 */
	template<typename UserClass>
	FORCEINLINE void AddAttackPattern(UserClass* Object, bool (UserClass::*ConditionFunctionPtr)(), void (UserClass::*FunctionPtr)(), const FString& InName)
	{
		FAttackExecuteDelegate NewHandler;
		NewHandler.BindUObject(Object, FunctionPtr);

		FSpecialAttackConditionDelegate NewConditionHandler;
		NewConditionHandler.BindUObject(Object, ConditionFunctionPtr);
		
		Patterns.Add(NewHandler);
		PatternConditions.Add(NewConditionHandler);
		PatternName.Add(InName);
		++PatternCount;

		LOG(TEXT("%s Pattern Added !"), *InName);
	}


protected:
	/** 특수공격 조건 델리게이트 */
	TArray<FSpecialAttackConditionDelegate> PatternConditions;

private:
	UPROPERTY()
	int8 PossibleSpecialPatternIndex;
	
};