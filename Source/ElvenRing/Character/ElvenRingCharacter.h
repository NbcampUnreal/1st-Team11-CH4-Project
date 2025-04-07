// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "ElvenRing/Interaction/InteractionComponent.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	UInteractionComponent* InteractionComponent;

protected:
	protected:
	//애니 몽타주(근데 이거 맞음??너무 더러운거보니까 잘못쓰고 있는거같은데)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DefenceMontage;
	//달리기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed; // 기본 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도

	//구르기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeCool = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDuration = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDistance = 1000.0f;
	float DodgeTime = 0.f;
	bool bIsDodging = false;
	FVector DodgeVelocity;
	//공격 방어 관련함수
	float AttackSpeed = 1.f;
	float DefenceSpeed = 1.f;
	bool bAttack;
	bool bDefence;
	FTimerHandle AttackTimerHandle;  
	FTimerHandle DefenceTimerHandle;  
	
	FVector DodgeStartLocation;
	FVector DodgeTargetLocation;
	
	FTimerHandle DodgeTimerHandle;  
	FTimerHandle DodgeStopTimerHandle;
	FTimerHandle DodgeStopTestTimerHandle;
	void PlayDodgeAnimation(float _Duration);
	void PlayAttackAnimation(float _AttackSpeed);
	void PlayDefenceAnimation(float _DefenceSpeed);
	void OnDefenceMontageEnded(UAnimMontage* Montage, bool bInterrupted);
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
	void StopDodge();
	UFUNCTION()
	void DodgeCollDown();
	UFUNCTION()
	void StartAttack(const FInputActionValue& value);
	UFUNCTION()
	void StopAttack();
	UFUNCTION()
	void StartDefence(const FInputActionValue& value);
	UFUNCTION()
	void StopDefence(const FInputActionValue& value);
	void Interact(const FInputActionValue& InputActionValue);
};

