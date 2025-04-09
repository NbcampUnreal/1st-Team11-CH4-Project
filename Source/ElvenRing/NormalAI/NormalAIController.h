#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalAIController.generated.h"

UCLASS()
class ELVENRING_API ANormalAIController : public AAIController
{
	GENERATED_BODY()

public:
	ANormalAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;


};
