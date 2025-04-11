#include "BossHangedMan.h"

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
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
