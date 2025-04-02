#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElvenRing/ElvenRing.h"
#include "BossPatternComponent.generated.h"

DECLARE_DELEGATE(FAttackExecuteDelegate);

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELVENRING_API UBossPatternComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBossPatternComponent();

	/** 공격 패턴 추가 함수. template 함수이므로 inline으로 구현*/
	template<typename UserClass>
	FORCEINLINE void AddAttackPattern(UserClass* Object, void (UserClass::*FunctionPtr)(), const FString& InName)
	{
		FAttackExecuteDelegate NewHandler;
		NewHandler.BindUObject(Object, FunctionPtr);
		
		Patterns.Add(NewHandler);
		PatternName.Add(InName);
		++PatternCount;

		LOG(TEXT("%s Pattern Added !"), *InName);
	};

	/** 공격 실행 순수 가상 함수 */
	virtual void ExecuteAttackPattern() PURE_VIRTUAL(UBossPatternComponent::ExecuteAttackPattern, );
	
protected:
	
private:

public:

protected:
	/** 등록된 공격 패턴 수 추적 변수 */
	UPROPERTY(VisibleInstanceOnly, Category = "Boss|Pattern")
	int8 PatternCount;
	
	/** 공격 실행 델리게이트 */
	TArray<FAttackExecuteDelegate> Patterns;

	/** 공격 패턴 이름 */
	TArray<FString> PatternName;

private:
	
};