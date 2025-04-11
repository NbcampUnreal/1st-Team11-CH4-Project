#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "BossHangedManHeart.generated.h"

UCLASS()
class ELVENRING_API ABossHangedManHeart : public AUnitBase
{
	GENERATED_BODY()

public:
	ABossHangedManHeart();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss")
	TObjectPtr<class ABossHangedMan> HangedMan;
};
