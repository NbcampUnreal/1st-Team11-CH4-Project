#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossHangedMan.generated.h"

UCLASS()
class ELVENRING_API ABossHangedMan : public ABoss
{
	GENERATED_BODY()

	public:
	ABossHangedMan();

	virtual void ServerOnSpawnSequenceEnded_Implementation() override;
	virtual void ServerOnPhaseSequenceEnded_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:


protected:
	

private:
	
};
