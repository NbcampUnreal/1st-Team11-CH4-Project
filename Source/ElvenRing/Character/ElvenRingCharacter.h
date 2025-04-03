// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "ElvenRingCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingCharacter : public AUnitBase
{
	GENERATED_BODY()

public:
	AElvenRingCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;
	protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DodgeMontage;
	//달리기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도

	//구르기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDuration = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDistance = 1000.0f;
	float DodgeTime = 0.f;
	bool bIsDodging = false;
	
	FVector DodgeStartLocation;
	FVector DodgeTargetLocation;
	
	FTimerHandle DodgeTimerHandle;  
	FTimerHandle DodgeStopTimerHandle;

	void PlayDodgeAnimation(float Duration);
	void UpdateDodge();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartDodge(const FInputActionValue& value);
	UFUNCTION()
	void StopDodge(); //혹시나 필요할거 같아서 일단 만듬
};

