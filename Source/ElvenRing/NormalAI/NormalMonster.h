#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "NormalMonster.generated.h"


UCLASS()
class ELVENRING_API ANormalMonster : public AUnitBase
{
	GENERATED_BODY()
public:
	ANormalMonster();


	// UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI")
	// TArray<AActor*> PatrolPoints;
};