#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Boss/Boss.h"
#include "BossArtorias.generated.h"

UCLASS()
class ELVENRING_API ABossArtorias : public ABoss
{
	GENERATED_BODY()


public:
	ABossArtorias();

protected:
	virtual void BeginPlay() override;

private:
	void HorizonSlashAttack();
	void VerticalSlashAttack();
	void RotationAttack();
	void DodgeAttack();
	void JumpAttack();
	bool JumpAttackCondition();
	void ThrustAttack();
	bool ThrustAttackCondition();
	void Dodge();

private:
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> HorizontalSlashAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> VerticalSlashAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> RotationAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> DodgeAttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> JumpAttackAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> DodgeAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> ThrustAnim;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinJumpAttackRadius;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinThrustAttackRadius;
};
