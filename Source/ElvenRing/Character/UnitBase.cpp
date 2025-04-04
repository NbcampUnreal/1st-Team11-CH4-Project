// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitBase.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AUnitBase::AUnitBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnitBase::Attack(AActor* Target)
{
}

void AUnitBase::OnAttacked()
{
}

void AUnitBase::OnDeath()
{
	OnDeathEvent.Broadcast(this);
}

void AUnitBase::OnHealthChanged()
{
}

float  AUnitBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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


void AUnitBase::PlayDamageAnim()
{
}

void AUnitBase::PlayDeathAnim()
{
}

// Called to bind functionality to input
void AUnitBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

