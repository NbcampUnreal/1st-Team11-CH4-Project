#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossTenebris.generated.h"

UCLASS()
class ELVENRING_API ABossTenebris : public ABoss
{
	GENERATED_BODY()

public:
	ABossTenebris();
	
protected:
	virtual void BeginPlay() override;

private:
	/** 1페이즈 공격 */
	void TailAttack();
	void EarthquakeAttack();
	void GrabAttack();

	/** 1페이즈 특수 공격 */
	void FlyingEarthquakeAttack();
	void BressAttackRight();
	
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

private:
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveFrontLeftAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveFrontAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackLeftAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackRightAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> MoveBackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> GrabAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> BressAttackFrontAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> BressAttackRightAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> EnergyAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> TailAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> EarthQuakeAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> RushAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> FlyingEarthquakeAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> FlyingRightFireBallAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> HowlingAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Attack")
	float FlyingEarthquakeAttackInterval;

	UPROPERTY(EditAnywhere, Category = "Boss|Attack")
	float BressAttackRightInterval;

	FTimerHandle FlyingEarthquakeAttackTimer;
	FTimerHandle BressAttackRightTimer;
};
