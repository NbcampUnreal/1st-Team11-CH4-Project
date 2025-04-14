// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitBase.generated.h"

UENUM(BlueprintType)
enum class ECharacterType : uint8 //ksw
{
	Player = 0,
	NormalMonster = 1,
	Boss = 2
};

UCLASS()
class ELVENRING_API AUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathDelegate, AUnitBase*, Unit);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnDeathDelegate OnDeathEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHpChangedDelegate, float, CurHp, float, MaxHp, int, State);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHpChangedDelegate OnHpChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStChangedDelegate, float, CurStamina, float, MaxStamina, int, State);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHpChangedDelegate OnStaminaChanged;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMpChangedDelegate, float, CurMp, float, MaxMp, int, State);
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHpChangedDelegate OnMpChanged;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool Invincibility = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_HealthChanged)
	float CurHealth;

	float CurStamina; //ksw
	float MaxStamina;//ksw
	float StaminaTime;//ksw
	float CurMana; //ksw
	float MaxMana; //ksw

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MoveSpeed;
	bool bCanAttack;
	bool bCanMove = true;
	bool bIsHit;
	bool bIsDie;
	bool bSprint; //ksw;
	void AttachDelegateToWidget(ECharacterType Type);//ksw;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(AActor* Target);
	virtual void OnAttacked();
	virtual void OnDeath();

	UFUNCTION(NetMulticast, reliable)
	virtual void OnRep_HealthChanged();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	float GetMaxHealth() const
	{
		return MaxHealth;
	}
	float CurGetHealth() const
	{
		return CurHealth;
	}
	float GetAttackPower() const
	{
		return AttackPower;
	}
	virtual void PlayDamageAnim();
	virtual void PlayDeathAnim();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
