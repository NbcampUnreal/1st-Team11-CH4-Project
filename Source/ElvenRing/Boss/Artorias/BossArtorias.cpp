#include "BossArtorias.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"

ABossArtorias::ABossArtorias()
{
	
}

void ABossArtorias::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();

	LOG(TEXT("Begin!"));

	NormalPattern->AddAttackPattern(this, &ABossArtorias::HorizonSlashAttack, FString("HorizonSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::VerticalSlashAttack, FString("VerticalSlashAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::RotationAttack, FString("RotationAttack"));
	NormalPattern->AddAttackPattern(this, &ABossArtorias::DodgeAttack, FString("DodgeAttack"));

	//NormalPattern->ExecuteAttackPattern();
}



void ABossArtorias::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void ABossArtorias::HorizonSlashAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(HorizontalSlashAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(HorizontalSlashAnim);
		}
	}
}



void ABossArtorias::VerticalSlashAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(VerticalSlashAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(VerticalSlashAnim);
		}
	}
}



void ABossArtorias::RotationAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(RotationAttackAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(RotationAttackAnim);	
		}
	}
}



void ABossArtorias::DodgeAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(DodgeAttackAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(DodgeAttackAnim);	
		}
	}
}



void ABossArtorias::JumpAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(JumpAttackAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(JumpAttackAnim);
		}
	}
}
