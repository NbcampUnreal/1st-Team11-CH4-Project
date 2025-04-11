#include "BossHangedMan.h"

#include "ElvenRing/ElvenRing.h"

ABossHangedMan::ABossHangedMan()
{
}

void ABossHangedMan::ServerOnSpawnSequenceEnded_Implementation()
{
	Super::ServerOnSpawnSequenceEnded_Implementation();
}

void ABossHangedMan::ServerOnPhaseSequenceEnded_Implementation()
{
	Super::ServerOnPhaseSequenceEnded_Implementation();
}

void ABossHangedMan::BeginPlay()
{
	Super::BeginPlay();
}

float ABossHangedMan::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CheckHealthPhase();
	return Damage;
}

void ABossHangedMan::CheckHealthPhase()
{
	if (CurHealth <= MaxHealth/4*3 && HealthPhase == EHealthPhase::None)
	{
		LOG(TEXT("3/4"));
		HealthPhase = EHealthPhase::First;
		PlayAnimation(GroggyAnim);
	}
	else if (CurHealth <= MaxHealth/2 && HealthPhase == EHealthPhase::First)
	{
		LOG(TEXT("1/2"));
		HealthPhase = EHealthPhase::Second;
		PlayAnimation(GroggyAnim);
	}
	else if (CurHealth <= MaxHealth/4 && HealthPhase == EHealthPhase::Second)
	{
		LOG(TEXT("1/4"));
		HealthPhase = EHealthPhase::Third;
		PlayAnimation(GroggyAnim);
	}
}
