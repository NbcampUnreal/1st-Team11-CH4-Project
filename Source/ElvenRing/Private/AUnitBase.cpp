// Fill out your copyright notice in the Description page of Project Settings.


#include "AUnitBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AAUnitBase::AAUnitBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAUnitBase::Attack()
{
}

void AAUnitBase::OnAttacked()
{
}

void AAUnitBase::OnDeath()
{
}

void AAUnitBase::OnHealthChanged()
{
}

float  AAUnitBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float HitDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Damage > 0 && !bIsDie)
	{
		CurHealth -= Damage;
		if (CurHealth <= 0.f) OnDeath();
		else 
		{
			PlayDamageAnim();
		}
	}
	return Damage;
}


void AAUnitBase::PlayDamageAnim()
{
}

void AAUnitBase::PlayDealthAnim()
{
}

// Called to bind functionality to input
void AAUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

