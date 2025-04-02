#pragma once

#include "CoreMinimal.h"
#include "BossStateInterface.h"
#include "Components/ActorComponent.h"
#include "BossAttackState.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELVENRING_API UBossAttackState : public UActorComponent, public IBossStateInterface
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
