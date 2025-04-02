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



void ABossArtorias::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);
}



void ABossArtorias::HorizonSlashAttack()
{
	if (IsValid(AnimInstance))
	{
		if (IsValid(HorizontalSlashAnim))
		{
			LOG(TEXT("Begin!"));
			AnimInstance->Montage_Play(HorizontalSlashAnim);
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABossArtorias::OnMontageEnded);
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
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABossArtorias::OnMontageEnded);
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
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABossArtorias::OnMontageEnded);
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
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABossArtorias::OnMontageEnded);
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
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABossArtorias::OnMontageEnded);
		}
	}
}