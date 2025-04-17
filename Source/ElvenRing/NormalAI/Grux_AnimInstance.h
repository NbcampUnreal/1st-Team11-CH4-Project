#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Grux_AnimInstance.generated.h"


UCLASS()
class ELVENRING_API UGrux_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UGrux_AnimInstance();
	
	UFUNCTION()
	void UpdateAttack(bool value);

	UFUNCTION()
	void UpdateHit(bool value);
	
	UFUNCTION()
	void UpdateDeath(bool value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool IsWaiting;

	FTimerHandle TimerHandle;


	UFUNCTION(BlueprintCallable)
	void AnimNotify_EndHit();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_EndAttack();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_AttackValue();
	bool InsSelect;
};
