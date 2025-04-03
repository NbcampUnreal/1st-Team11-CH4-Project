#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NormalAIController.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API ANormalAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	 virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// UFUNCTION(BlueprintCallable, Category="AI")
	// void MoveToCurrentPatrolPoint();

protected:
	// 현재 순찰 지점 인덱스
	//int32 CurrentPatrolIndex = 0;
};
