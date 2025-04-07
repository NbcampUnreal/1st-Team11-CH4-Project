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
	void Howling(); // 이거 어떻게 활용하지 
	void BressAttackFront();
	void EnergyAttack();

	/** 2페이즈 발동 시 특수 공격 */
	void FlyingRightFireBallAttack();

	void MoveFrontLeft();
	void MoveFront();
	void MoveBackLeft();
	void MoveBackRight();
	void MoveBack();
	
	void RushAttack(); // 없어도 될 것 같은 느낌. 플레이어를 불쾌하게 만들 가능성 존재.
	void WalkingFront();

	void SetSpecialAttackTimer(int8 index);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	FName BressCollisionSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCapsuleComponent* BressAttackCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Stat")
	TArray<FName> TailCollisionSocketName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<UCapsuleComponent*> TailAttackCollisions;

private:
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

	FTimerHandle SpecialAttackTimer;

	ETenebrisSpecialAttackType AttackType;
	EPhaseType PhaseType;
};