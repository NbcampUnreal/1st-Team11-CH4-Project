#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "ElvenRing/Boss/Boss.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"
#include "BossHangedMan.generated.h"

class ATargetPoint;

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
	
protected:
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	/** 보스의 현재 체력에 따라 그로기 애니메이션 출력 판정 */
	void CheckHealthPhase();
	
	UFUNCTION(BlueprintCallable)
	void LightningAttack();

	void ElectricJavelinAttack();

	void ThunderBoldAttack();

	void ElectronicAttackUp();
	void ElectronicAttackDown();

	void SpecialAttack();

	void Groggy();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLightning();

	void GetAllPlayersLocation();

protected:
	

private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Pattern")
	TSubclassOf<AActor> LightningNiagara;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Pattern")
	float LightningStartDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Pattern")
	float LightningInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Pattern")
	int8 MaxLightningCount;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> LightningAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> GroggyAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> ElectricJavelinAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> ThunderBoldAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> ElectronicUpAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> ElectronicDownAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<UAnimMontage> SpecialAttackAnim;
	
	EHealthPhase HealthPhase;
	
	FTimerHandle LightningTimer;

	int8 CurrentLightningCount;

	TArray<FVector> PlayersLocation;
	int8 PlayerCount;
};