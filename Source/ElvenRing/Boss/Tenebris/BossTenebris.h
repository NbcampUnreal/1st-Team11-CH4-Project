#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossTenebris.generated.h"

UENUM()
enum class ETenebrisSpecialAttackType : uint8
{
	None,
	FlyingEarthquake,
	BressRight
};

UENUM()
enum class EPhaseType : uint8
{
	One,
	Two
};

UCLASS()
class ELVENRING_API ABossTenebris : public ABoss
{
	GENERATED_BODY()

public:
	ABossTenebris();

	virtual void OnSpawnSequenceEnded() override;
	virtual void OnPhaseSequenceEnded() override;
	
protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	
	/** 1페이즈 공격 */
	void TailAttack();
	void EarthquakeAttack();
	void GrabAttack();

	/** 1페이즈 특수 공격 */
	void FlyingEarthquakeAttack();
	bool FlyingEarthquakeAttackCondition();
	void BressAttackRight();
	bool BressAttackRightCondition();
	
	/** 2페이즈 공격 */
	void Howling();  
	void BressAttackFront();
	void EnergyAttack();

	/** 2페이즈 발동 시 특수 공격 */
	void FlyingRightFireBallAttack();
	
	void RushAttack();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	FName BressCollisionSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* BressAttackCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	TArray<FName> TailCollisionSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UCapsuleComponent*> TailAttackCollisions;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	FName GrabCollisionSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* GrabAttackCollision;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|BGM")
	USoundBase* BossBattleBGM2;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> BressAfterMoveFrontAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveFrontLeftAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveFrontAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackLeftAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackRightAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> GrabAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> BressAttackFrontAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> BressAttackRightAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> EnergyAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> TailAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> EarthQuakeAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> RushAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> FlyingEarthquakeAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> FlyingRightFireBallAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> HowlingAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> WalkingFrontAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	float SpecialAttackInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Sequence")
	TSubclassOf<ANormalLevelSequenceActor> LevelSequence;

	UPROPERTY(EditInstanceOnly, Category = "Boss|Sequence")
	EPhaseType PhaseType;

	FTimerHandle SpecialAttackTimer;

	ETenebrisSpecialAttackType AttackType;
};