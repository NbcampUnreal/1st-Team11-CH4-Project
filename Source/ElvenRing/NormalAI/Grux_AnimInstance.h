#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Grux_AnimInstance.generated.h"

UCLASS()
class ELVENRING_API UGrux_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
UGrux_AnimInstance();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsDie;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsWaiting;

	
};