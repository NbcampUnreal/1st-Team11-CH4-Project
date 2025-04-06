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
	
	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void Attack(AActor* Target) override;

	virtual void BeginPlay() override;
};






