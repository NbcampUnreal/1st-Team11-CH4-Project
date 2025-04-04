#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossBlackBear.generated.h"

UCLASS()
class ELVENRING_API ABossBlackBear : public ABoss
{
	GENERATED_BODY()

public:
	ABossBlackBear();

protected:
	virtual void BeginPlay() override;

private:
	void JumpAttack1();
	void JumpAttack2();
	void TwoGrabAttack();
	void TwoComboOneGrabAttack();
	void OneComboOneGrabAttack();
	void ThreeComboAttack1();
	void ThreeComboAttack2();

private:
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> JumpAttack1Anim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> JumpAttack2Anim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> TwoGrabAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> TwoComboOneGrabAttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> OneComboOneGrabAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> ThreeComboAttack1Anim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> ThreeComboAttack2Anim;
};
