#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "NormalLevelSequenceActor.generated.h"

class ULevelSequence;
class ABoss;

UENUM(BlueprintType)
enum class ESequenceType : uint8
{
	Battle,
	Dead
};

UCLASS()
class ELVENRING_API ANormalLevelSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartSequence();
	
	UFUNCTION()
	void OnSequenceEnded();
	
	void SetAllPlayerHidden();
	void SetAllPlayerUnhidden();
	void SetBossBattleModeWithDelay();
	

private:
	UPROPERTY(EditAnywhere, Category = "Boss")
	float BattleModeDelay = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss")
	ULevelSequence* CurrentLevelSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	ESequenceType SequenceType;
};
