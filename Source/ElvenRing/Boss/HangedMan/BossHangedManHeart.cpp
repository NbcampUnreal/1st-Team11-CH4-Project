#include "BossHangedManHeart.h"
#include "BossHangedMan.h"
#include "Kismet/GameplayStatics.h"

ABossHangedManHeart::ABossHangedManHeart()
{
	HangedMan = nullptr;
}

float ABossHangedManHeart::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UGameplayStatics::ApplyDamage(HangedMan, Damage, EventInstigator, DamageCauser, UDamageType::StaticClass());
	
	return Damage;
}
