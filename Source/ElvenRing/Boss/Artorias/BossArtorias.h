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

	virtual void ServerOnSpawnSequenceEnded_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

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
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> HorizontalSlashAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> VerticalSlashAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> RotationAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> DodgeAttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> JumpAttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> DodgeAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Boss|Anim")
	TObjectPtr<class UAnimMontage> ThrustAnim;
	
	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinJumpAttackRadius;

	UPROPERTY(EditAnywhere, Category = "Boss|Stat")
	float MinThrustAttackRadius;
};
