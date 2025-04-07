// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitBase.h"

#include "ElvenRing/ElvenRing.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/UI/PlayerMainUi.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"

// Sets default values
AUnitBase::AUnitBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurStamina = 100;//ksw
	MaxStamina = 100;
	CurMana = 100;
	MaxMana = 100;
}

// Called when the game starts or when spawned
void AUnitBase::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitBase::AttachDelegateToWidget(ECharacterType Type)
{
	GetGameInstance<UElvenringGameInstance>()->BindToCharacterOpenWidget(Type, this);
}

// Called every frame
void AUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Sprint Test용도로 만든거라 따로 구현하시던지 삭제 해도됩니다. ksw
	if (bSprint)
	{
		CurStamina -= 3.f * DeltaTime;
		CurStamina = FMath::Clamp(CurStamina, 0, MaxStamina);
		OnStaminaChanged.Broadcast(CurStamina, MaxStamina, 0);//ksw
		StaminaTime = 0.f;
	}
	else
	{
		if (100 > CurStamina)
		{
			StaminaTime += DeltaTime;
			if (2.f < StaminaTime)
			{
				CurStamina += 5.f * DeltaTime;
				CurStamina = FMath::Clamp(CurStamina, 0, MaxStamina);
				OnStaminaChanged.Broadcast(CurStamina, MaxStamina, 1);//ksw
				UE_LOG(LogTemp, Warning, TEXT("CurStamina = %f"), CurStamina);
			}
		}
	}
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
	bIsDie = true;
}

void AUnitBase::OnHealthChanged()
{
	OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
}

float  AUnitBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float HitDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (Damage > 0 && !bIsDie)
	{
		// 연출을 위해 원래 데미지 주변 값으로 계산
		const float InterpDamage =  FMath::RandRange(Damage-Damage/5, Damage+Damage/5);
		
		CurHealth -= InterpDamage;
		LOG(TEXT("Get Damaged ! Current Health : %f"), CurHealth);
		if (CurHealth <= 0.f) OnDeath();
		else
		{
			PlayDamageAnim();
		}
		OnHealthChanged();//ksw
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

