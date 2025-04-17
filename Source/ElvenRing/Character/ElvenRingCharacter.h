// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitBase.h"
#include "BaseWeapon.h"
#include "NiagaraSystem.h"
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
	void ToggleInput(bool bInput);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	bool bInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* DieSound;
	virtual void OnDeath() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(blueprintcallable)
	void HandleDeath();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* HitNiagara;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackAnimation(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Death(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Hit(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Heal(UAnimMontage* Montage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDodgeAnimation(float _DodgeDuration);
	//공격 함수 및 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack",Replicated)
	int AttackIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack",Replicated)
	bool bIsAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bCanCombo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimMontage* AttackMontage3;
	FTimerHandle ComboTimerHandle;
	void OnAttackInput();
	UFUNCTION(Server, Reliable)
	void Server_OnAttackInput();
	void OnDodgeInput(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void Server_OnDodgeInput(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void Server_Heal();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void OnAttackAnimationEnd();
	void ComboEnd();
	void ResetCombo();
	void PlayAttackAnimation();
	//무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseWeapon* CurrentWeapon;	

	
	//애니 몽타주(근데 이거 맞음??너무 더러운거보니까 잘못쓰고 있는거같은데)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DefenceMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* DieMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* HealMontage;
	//달리기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;  // "기본 속도" 대비 몇 배로 빠르게 달릴지 결정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed; 	// 실제 스프린트 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement",Replicated)
	bool IsSprint = false;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartSprint();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopSprint();
	//구르기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeCool = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDuration = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeDistance = 3000.0f;
	float DodgeTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	bool bIsDodging = false;
	FVector DodgeVelocity;
	float OriginalMaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dodge")
	float DodgeSpeed;
	//방어 관련함수
	float DefenceSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defence")
	bool bDefence;
	FTimerHandle DefenceTimerHandle;  
	
	FVector DodgeStartLocation;
	FVector DodgeTargetLocation;
	
	FTimerHandle DodgeTimerHandle;  
	FTimerHandle JumpTimerHandle;  
	FTimerHandle DodgeStopTimerHandle;
	FTimerHandle DodgeStopTestTimerHandle;

	UFUNCTION(BlueprintCallable,Category="Jump")
	void SetbJump(bool _bJump);
	FVector2D MoveInput;
	bool bJump = false;
	bool bdodge = false;
	void PlayDodgeAnimation(float _Duration);
	void PlayDefenceAnimation(float _DefenceSpeed);
	void OnDefenceMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void UpdateDodge();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void Heal(const FInputActionValue& value);
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void MoveEnd(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	void SetBoolTrue();
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
	void StartDefence(const FInputActionValue& value);
	UFUNCTION()
	void StopDefence(const FInputActionValue& value);
	void Interact(const FInputActionValue& InputActionValue);
	
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;//ksw
	virtual void BeginPlay() override;//ksw
};


