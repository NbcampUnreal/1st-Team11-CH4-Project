#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BossStateInterface.generated.h"

class ABoss;

UINTERFACE(MinimalAPI)
class UBossStateInterface : public UInterface
{
	GENERATED_BODY()
};

class ELVENRING_API IBossStateInterface
{
	GENERATED_BODY()

public:
	/** 특정 상태에 진입 시 호출하는 함수 */
	virtual void OnStateEnter(ABoss* Boss) = 0;
	
	/** 특정 상태에서 반복적으로 호출하는 함수 */
	virtual void OnStateUpdate() = 0;
	
	/** 툭정 상태가 끝날 때 호출하는 함수 */
	virtual  void OnStateExit() = 0;
};
