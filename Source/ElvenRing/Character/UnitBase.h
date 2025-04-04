// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitBase.generated.h"

UCLASS()
class ELVENRING_API AUnitBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MaxHealth;
	float CurHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float MoveSpeed;
	bool bCanAttack;
	bool bCanMove;
	bool bIsHit;
	bool bIsDie;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Attack(AActor* Target);
	virtual void OnAttacked();
	virtual void OnDeath();
	virtual void OnHealthChanged();
	virtual float TakeDamage
	(
		float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
	) override;
	float GetMaxHealth() const
	{
		return MaxHealth;
	}
	float CurGetHealth() const
	{
		return CurHealth;
	}

	virtual void PlayDamageAnim();
	virtual void PlayDeathAnim();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
