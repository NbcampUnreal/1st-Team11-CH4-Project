#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ObjectRename.h"
#include "BBNormalAI.generated.h"

UCLASS()
class ELVENRING_API UBBNormalAI : public UBlackboardComponent
{
	GENERATED_BODY()

public:
	UBBNormalAI();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FName PlayerDetectedKey = "PlayerDetectedKey";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FName AttackKey = "Attack";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FName WaitingKey = "Waiting";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
	FName TargetActorKey = "TargetActor";
};