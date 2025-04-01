// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UnitBase.generated.h"

UCLASS()
class ELVENRING_API AUnitBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float MaxHealth;
	float CurHealth;
	float AttackPower;
	float MoveSpeed;
	bool bCanAttack;
	bool bCanMove;
	bool bIsHit;
	bool bIsDie;

public:
	virtual void Tick(float DeltaTime) override;
	void Attack();
	void OnAttacked();
	void OnDeath();
	void OnHealthChanged();
	virtual float TakeDamage
	(
		float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser
	) override;
	float MaxGetHealth() const
	{
		return MaxHealth;
	}
	float CurGetHealth() const
	{
		return CurHealth;
	}

	virtual void PlayDamageAnim();
	virtual void PlayDealthAnim();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
