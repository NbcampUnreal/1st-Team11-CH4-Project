#pragma once

#include "CoreMinimal.h"
#include "BossStateInterface.h"
#include "BossMoveState.generated.h"

UCLASS()
class ELVENRING_API UABossMoveState : public UActorComponent, public IBossStateInterface
{
	GENERATED_BODY()

public:
	virtual void OnStateEnter(ABoss* Boss) override;
	virtual void OnStateUpdate() override;
	virtual void OnStateExit() override;

private:
	UPROPERTY()
	ABoss* CurrentBoss;
	
};
