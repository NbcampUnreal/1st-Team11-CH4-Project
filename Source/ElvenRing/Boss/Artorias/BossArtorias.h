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
	/** 수평으로 검을 휘두르는 공격 */
	void HorizonSlashAttack();

	/** 위에서 아래로 검을 휘두르는 공격 */
	void VerticalSlashAttack();

	/** 한 바퀴 회전하며 검을 휘두르는 공격 */
	void RotationAttack();

	/** 검을 휘두름과 동시에 후진 방향으로 구르는 패턴 */
	void DodgeAttack();

	/** 높게 뛰어올라 플레이어에게 착지하며 검을 휘두르는 공격 */
	void JumpAttack();

	/** JumpAttack을 실행하기 위한 조건 함수 */
	bool JumpAttackCondition();

	/** 플레이어에게 급속도로 전진하며 검을 찌르는 공격 */
	void ThrustAttack();

	/** ThrustAttack을 실행하기 위한 조건 함수 */
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
