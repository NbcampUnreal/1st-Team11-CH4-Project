#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
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

	UFUNCTION(BlueprintCallable, Category="AI")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category="AI")
	void PlayerDetected(UObject* TargetObject);


	virtual void BeginPlay() override;
	virtual void OnDeath() override;

	void SetWidget(UUserWidget* Widget); //ksw
	void UpdateHPBar();

	UPROPERTY(ReplicatedUsing = OnRep_InstanceIsHit)
	bool InstanceIsHit;

	UPROPERTY(ReplicatedUsing = OnRep_InstanceIsAttack)
	bool InstanceIsAttack;

	UPROPERTY(ReplicatedUsing = OnRep_InstanceIsDeath)
	bool InstanceIsDeath;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_InstanceIsHit();

	UFUNCTION()
	void OnRep_InstanceIsAttack();

	UFUNCTION()
	void OnRep_InstanceIsDeath();

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
