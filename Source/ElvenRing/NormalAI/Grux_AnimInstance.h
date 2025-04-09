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
	bool IsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsWaiting;

	FTimerHandle TimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void AttackAnim();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void HitAnim();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void DeathAnim();

	//추후 복잡한 노티파이는 c++로 작성
	// UFUNCTION()
	// void AnimNotify_ChangeBool();
};
