#include "BossBlackBear.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/Artorias/BossArtorias.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"

ABossBlackBear::ABossBlackBear()
{
}

void ABossBlackBear::BeginPlay()
{
	LOG(TEXT("Begin!"));

	/*NormalPattern->AddAttackPattern(this, &ABossBlackBear::JumpAttack1, FString("JumpAttack1"));
	NormalPattern->AddAttackPattern(this, &ABossBlackBear::JumpAttack2, FString("JumpAttack2"));*/
	NormalPattern->AddAttackPattern(this, &ABossBlackBear::TwoGrabAttack, FString("TwoGrabAttack"));
	/*NormalPattern->AddAttackPattern(this, &ABossBlackBear::TwoComboOneGrabAttack, FString("TwoComboOneGrabAttack"));
	NormalPattern->AddAttackPattern(this, &ABossBlackBear::OneComboOneGrabAttack, FString("OneComboOneGrabAttack"));
	NormalPattern->AddAttackPattern(this, &ABossBlackBear::ThreeComboAttack1, FString("ThreeComboAttack1"));
	NormalPattern->AddAttackPattern(this, &ABossBlackBear::ThreeComboAttack2, FString("ThreeComboAttack2"));*/

	
	Super::BeginPlay();

	SetBossBattleMode();
}

void ABossBlackBear::JumpAttack1()
{
	PlayAnimMontage(JumpAttack1Anim);
}

void ABossBlackBear::JumpAttack2()
{
	PlayAnimMontage(JumpAttack2Anim);
}

void ABossBlackBear::TwoGrabAttack()
{
	PlayAnimMontage(TwoGrabAttackAnim);
}

void ABossBlackBear::TwoComboOneGrabAttack()
{
	PlayAnimMontage(TwoComboOneGrabAttackAnim);
}

void ABossBlackBear::OneComboOneGrabAttack()
{
	PlayAnimMontage(OneComboOneGrabAttackAnim);
}

void ABossBlackBear::ThreeComboAttack1()
{
	PlayAnimMontage(ThreeComboAttack1Anim);
}

void ABossBlackBear::ThreeComboAttack2()
{
	PlayAnimMontage(ThreeComboAttack2Anim);
}
