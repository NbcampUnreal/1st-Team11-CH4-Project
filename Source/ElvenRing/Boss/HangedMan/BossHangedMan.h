#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossHangedMan.generated.h"

UENUM()
enum class EHealthPhase : uint8
{
	None,
	First,
	Second,
	Third
};

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
	/** 보스의 현재 체력에 따라 그로기 애니메이션 출력 판정 */
	void CheckHealthPhase();

protected:
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> GroggyAnim;
	
	EHealthPhase HealthPhase;
	
};
