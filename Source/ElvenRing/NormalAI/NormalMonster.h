#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "NormalMonster.generated.h"

class UWidgetComponent; //ksw
class ElvenrinCharacter;

UCLASS()
class ELVENRING_API ANormalMonster : public AUnitBase
{
	GENERATED_BODY()

public:
	ANormalMonster();

	void UpdateHPBar();
	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void Attack(AActor* Target) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="AI")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void PlayDamageAnim() override;
	
	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void PlayDeathAnim() override;

	virtual void OnDeath() override;

	void SetWidget(UUserWidget* Widget);//ksw

	void UpdateAnim();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UWidgetComponent* HPWidgetComponent;
	FTimerHandle UpdateHPBarTimer;
	FTimerHandle StayTimer;

	UFUNCTION(BlueprintCallable, Category="AI")
	void PlayerDetected(UObject* TargetObject);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool bisHit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	bool bIsDie;

};	






