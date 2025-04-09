#include "BossArtorias.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"

ABossArtorias::ABossArtorias()
{
	MinJumpAttackRadius = 1000.0f;
	MinThrustAttackRadius = 700.0f;
}



void ABossArtorias::BeginPlay()
{
	LOG(TEXT("Begin!"));

	NormalPattern->AddAttackPattern(this, &ABossArtorias::HorizonSlashAttack, FString("HorizonSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::VerticalSlashAttack, FString("VerticalSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::RotationAttack, FString("RotationAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::DodgeAttack, FString("DodgeAttack"));

	SpecialPattern->AddAttackPattern(this, &ABossArtorias::JumpAttackCondition, &ABossArtorias::JumpAttack, FString("JumpAttack"));
	SpecialPattern->AddAttackPattern(this, &ABossArtorias::ThrustAttackCondition, &ABossArtorias::ThrustAttack, FString("ThrustAttack"));

	//NormalPattern->AddAttackPattern(this, &ABossArtorias::Dodge, FString("Dodge"));
	
	Super::BeginPlay();
}



void ABossArtorias::OnSpawnSequenceEnded()
{
	SetBossBattleMode();
}



void ABossArtorias::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToTarget(DeltaTime);
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

bool ABossArtorias::JumpAttackCondition()
{
	if (GetDistanceBetweenTarget() >= MinJumpAttackRadius)
	{
		return true;
	}

	return false;
}

void ABossArtorias::ThrustAttack()
{
	PlayAnimMontage(ThrustAnim);
}

bool ABossArtorias::ThrustAttackCondition()
{
	if (GetDistanceBetweenTarget() >= MinThrustAttackRadius)
	{
		return true;
	}

	return false;
}


void ABossArtorias::Dodge()
{
	PlayAnimMontage(DodgeAnim);
}
