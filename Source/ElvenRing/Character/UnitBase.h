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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_HealthChanged)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bCanMove = true;
	bool bIsHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	bool bIsDie;
	bool bSprint; //ksw;
	void AttachDelegateToWidget(ECharacterType Type);//ksw;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(AActor* Target);
	virtual void OnAttacked();
	virtual void OnDeath();

	UFUNCTION()
	virtual void OnRep_HealthChanged();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	float GetMaxHealth() const
	{
		return MaxHealth;
	}
	float GetCurHealth() const
	{
		return CurHealth;
	}
	float GetCurStamina() const
	{
		return CurStamina;
	}
	float GetCurMana() const
	{
		return CurMana;
	}
	float GetAttackPower() const
	{
		return AttackPower;
	}
	// 직접적으로 내부 Property를 외부로 노출하는 것은 좋지 않지만 시간이 없으므로 현재는 Setter를 이용해서 직접적으로 수정하게 한다.
		
	void SetCurrentHealth(float NewHealth)
	{
		CurHealth = NewHealth;
	}
	void SetCurrentStamina(float NewStamina)
	{
		CurStamina = NewStamina;
	}
	void SetCurrentMana(float NewMana)
	{
		CurMana = NewMana;
	}
	virtual void PlayDamageAnim();
	virtual void PlayDeathAnim();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
