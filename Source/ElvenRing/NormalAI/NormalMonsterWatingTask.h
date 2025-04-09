#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "NormalMonsterWatingTask.generated.h"

class ANormalMonster;


UCLASS()
class ELVENRING_API UNormalMonsterWatingTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UNormalMonsterWatingTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	static void UpdateWaiting(ANormalMonster* NormalMonster);
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
