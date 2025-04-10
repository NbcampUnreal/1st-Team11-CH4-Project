#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Net/UnrealNetwork.h"
#include "NormalMonster.generated.h"


class UWidgetComponent; //ksw
class ElvenrinCharacter;

UCLASS()
class ELVENRING_API ANormalMonster : public AUnitBase
{
	GENERATED_BODY()

public:
	ANormalMonster();

	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void Attack(AActor* Target) override;

	UFUNCTION(BlueprintCallable, Category="AI")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category="AI")
	void PlayerDetected(UObject* TargetObject);

	UFUNCTION()
	void OnRep_CurHealth();
	
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual void PlayDeathAnim() override;

	void SetWidget(UUserWidget* Widget);//ksw
	void UpdateHPBar();
	
	// virtual void GetLifetimeReplicatedProps
	// (TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//
	// UPROPERTY(ReplicatedUsing = OnRep_CurHealth) // 값 변경 시 클라이언트에서 호출
	// float CurHealth;

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UWidgetComponent* HPWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool bisHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	bool bIsDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackAngle;

	
	
	FTimerHandle UpdateHPBarTimer;
	FTimerHandle StayTimer;

	
	
	
	

};	






