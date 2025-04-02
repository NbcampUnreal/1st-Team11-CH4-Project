#include "BossArtorias.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"

ABossArtorias::ABossArtorias()
{
	
}



void ABossArtorias::BeginPlay()
{
	LOG(TEXT("Begin!"));

	NormalPattern->AddAttackPattern(this, &ABossArtorias::HorizonSlashAttack, FString("HorizonSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::VerticalSlashAttack, FString("VerticalSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::RotationAttack, FString("RotationAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::DodgeAttack, FString("DodgeAttack"));
	
	Super::BeginPlay();
}



void ABossArtorias::HorizonSlashAttack()
{
	PlayAnimMontage(HorizontalSlashAnim);
}



void ABossArtorias::VerticalSlashAttack()
{
	PlayAnimMontage(VerticalSlashAnim);
}



void ABossArtorias::RotationAttack()
{
	PlayAnimMontage(RotationAttackAnim);
}



void ABossArtorias::DodgeAttack()
{
	PlayAnimMontage(DodgeAttackAnim);
}



void ABossArtorias::JumpAttack()
{
	PlayAnimMontage(JumpAttackAnim);
}