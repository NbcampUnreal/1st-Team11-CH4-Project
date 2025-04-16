// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitBase.h"

#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/Boss.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/UI/PlayerMainUi.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/Core/ElvenRingGameMode.h"
#include "ElvenRing/UI/UIManager.h"
#include "Net/UnrealNetwork.h"

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

void AUnitBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUnitBase, CurHealth);
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
				//UE_LOG(LogTemp, Warning, TEXT("CurStamina = %f"), CurStamina);
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

void AUnitBase::OnRep_HealthChanged()
{
	// 로컬 플레이어인 경우에만 UI 업데이트
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn() == this)
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
	else if (Cast<ABoss>(this))
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
}

float  AUnitBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority()) return 0;
	if (Invincibility)
	{
		if (AElvenRingGameMode* GameMode = GetWorld()->GetAuthGameMode<AElvenRingGameMode>())
		{
			GameMode->RecordInvincible(EventInstigator, this, Damage);
		}
		return 0;
	}
	
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0 && !bIsDie)
	{
		// 연출을 위해 원래 데미지 주변 값으로 계산
		ActualDamage =  FMath::RandRange(Damage-Damage/5, Damage+Damage/5);
		
		CurHealth -= ActualDamage;
		//LOG(TEXT("Get Damaged ! Current Health : %f"), CurHealth);
		if (CurHealth <= 0.f) OnDeath();
		else
		{
			PlayDamageAnim();
		}
		OnRep_HealthChanged();//ksw

		if (AElvenRingGameMode* GameMode = GetWorld()->GetAuthGameMode<AElvenRingGameMode>())
		{
			GameMode->RecordDamage(EventInstigator, this, ActualDamage);
		}
	}
	
	return ActualDamage;
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

