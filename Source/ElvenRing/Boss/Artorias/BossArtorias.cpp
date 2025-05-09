#include "BossArtorias.h"

#include "Components/AudioComponent.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/BossWidget.h"
#include "ElvenRing/UI/UIManager.h"

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
	Super::OnSpawnSequenceEnded();
	UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->GetUIManager()->GetBossWidgetUi()->SetName("Artorias, The Guardian Of Dark Castle");
	if (HasAuthority())
	{
		SetBossBattleMode();
		PlaySound(BossBattleBGM);
	}
}

void ABossArtorias::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToTarget(DeltaTime);
}

void ABossArtorias::HorizonSlashAttack()
{
	PlayAnimation(HorizontalSlashAnim);
}

void ABossArtorias::VerticalSlashAttack()
{
	PlayAnimation(VerticalSlashAnim);
}

void ABossArtorias::RotationAttack()
{
	PlayAnimation(RotationAttackAnim);
}

void ABossArtorias::DodgeAttack()
{
	PlayAnimation(DodgeAttackAnim);
}

void ABossArtorias::JumpAttack()
{
	PlayAnimation(JumpAttackAnim);
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
	PlayAnimation(ThrustAnim);
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
	PlayAnimation(DodgeAnim);
}