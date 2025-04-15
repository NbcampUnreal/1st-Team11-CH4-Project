// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Rampage_AnimInstance.generated.h"

UCLASS()
class ELVENRING_API URampage_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	URampage_AnimInstance();

public:
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
};
