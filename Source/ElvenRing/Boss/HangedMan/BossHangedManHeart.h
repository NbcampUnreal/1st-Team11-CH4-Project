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

	TObjectPtr<class ABossHangedMan> HangedMan;
};
