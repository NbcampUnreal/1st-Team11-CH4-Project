#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Net/UnrealNetwork.h"
#include "NormalMonster.generated.h"


class UWidgetComponent; //ksw

UCLASS()
class ELVENRING_API ANormalMonster : public AUnitBase
{
	GENERATED_BODY()

public:
	ANormalMonster();

	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void Attack(AActor* Target) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category="AI")
	void PlayerDetected(UObject* TargetObject);


	virtual void BeginPlay() override;
	virtual void OnDeath() override;

	void SetWidget(UUserWidget* Widget); //ksw
	void UpdateHPBar();

	bool InstanceIsHit;
	bool InstanceIsAttack;
	bool InstanceIsDeath;
	
	UFUNCTION(Server, Reliable)
	void RPCIsHit(bool value);

	UFUNCTION(Server, Reliable)
	void RPCIsAttack(bool value);
	
	UFUNCTION(Server, Reliable)
	void RPCIsDeath(bool value);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsHit(bool value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsAttack(bool value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsDeath(bool value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UWidgetComponent* HPWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool MonsterIsHit;

	FTimerHandle UpdateHPBarTimer;
	FTimerHandle StayTimer;
};
