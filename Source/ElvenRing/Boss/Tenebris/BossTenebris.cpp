#include "BossTenebris.h"

#include "Components/CapsuleComponent.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"

ABossTenebris::ABossTenebris()
{
	AttackType = ETenebrisSpecialAttackType::None;
	PhaseType = EPhaseType::One;

	SpecialAttackInterval = 40;

	// 보스 Bress 공격 판정 콜리전
	BressAttackCollision = CreateDefaultSubobject<UCapsuleComponent>("BressAttack Collision");
	BressAttackCollision->SetupAttachment(GetMesh());
	
	UCapsuleComponent* TailAttackCollision1 = CreateDefaultSubobject<UCapsuleComponent>("TailAttack Collision1");
	TailAttackCollision1->SetupAttachment(GetMesh());

	UCapsuleComponent* TailAttackCollision2 = CreateDefaultSubobject<UCapsuleComponent>("TailAttack Collision2");
	TailAttackCollision2->SetupAttachment(GetMesh());

	TailAttackCollisions.Add(TailAttackCollision1);
	TailAttackCollisions.Add(TailAttackCollision2);

}

void ABossTenebris::BeginPlay()
{
	LOG(TEXT("Begin!"));

	//NormalPattern->AddAttackPattern(this, &ABossTenebris::GrabAttack, FString("GrabAttack"));
	NormalPattern->AddAttackPattern(this, &ABossTenebris::TailAttack, FString("TailAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::EarthquakeAttack, FString("EarthquakeAttack"));

	//NormalPattern->AddAttackPattern(this, &ABossTenebris::BressAttackFront, FString("BressAttackFront"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::EnergyAttack, FString("EnergyAttack"));
	
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::FlyingRightFireBallAttack, FString("FlyingRightFireBallAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::Howling, FString("Howling"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBackLeft, FString("MoveBackLeft"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBackRight, FString("MoveBackRight"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBack, FString("MoveBack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveFront, FString("MoveFront"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveFrontLeft, FString("MoveFrontLeft"));

	//SpecialPattern->AddAttackPattern(this, &ABossTenebris::BressAttackRightCondition, &ABossTenebris::BressAttackRight, FString("BressAttackRight"));
	//SpecialPattern->AddAttackPattern(this, &ABossTenebris::FlyingEarthquakeAttackCondition, &ABossTenebris::FlyingEarthquakeAttack, FString("FlyingEarthquakeAttack"));

	//NormalPattern->AddAttackPattern(this, &ABossTenebris::RushAttack, FString("RushAttack"));
	
	RegisterCollision(BressAttackCollision, BressCollisionSocketName);
	RegisterCollision(TailAttackCollisions[0], TailCollisionSocketName[0]);
	RegisterCollision(TailAttackCollisions[1], TailCollisionSocketName[1]);

	Super::BeginPlay();

	SetBossBattleMode();

	/*PlayAnimMontage(BressAfterMoveFrontAnim);
	SetSpecialAttackTimer(0);*/
}

float ABossTenebris::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (PhaseType != EPhaseType::One) return Damage;
		
	if (CurHealth < MaxHealth/2)
	{
		if (PhaseType == EPhaseType::One)
		{
			LOG(TEXT("Begin"));
			PhaseType = EPhaseType::Two;
		}
	}

	return Damage;
}

void ABossTenebris::GrabAttack()
{
	PlayAnimMontage(GrabAttackAnim);
}

void ABossTenebris::BressAttackFront()
{
	PlayAnimMontage(BressAttackFrontAnim);
}

void ABossTenebris::BressAttackRight()
{
	PlayAnimMontage(BressAttackRightAnim);
	AttackType = ETenebrisSpecialAttackType::None;
	SetSpecialAttackTimer(1);
}

bool ABossTenebris::BressAttackRightCondition()
{
	if (AttackType == ETenebrisSpecialAttackType::BressRight)
	{
		return true;
	}
	
	return false;
}

void ABossTenebris::EnergyAttack()
{
	PlayAnimMontage(EnergyAttackAnim);
}

void ABossTenebris::TailAttack()
{
	PlayAnimMontage(TailAttackAnim);
}

void ABossTenebris::EarthquakeAttack()
{
	PlayAnimMontage(EarthQuakeAttackAnim);
}

void ABossTenebris::RushAttack()
{
	PlayAnimMontage(RushAttackAnim);
}

void ABossTenebris::WalkingFront()
{
	PlayAnimMontage(WalkingFrontAnim);
}


void ABossTenebris::FlyingEarthquakeAttack()
{
	PlayAnimMontage(FlyingEarthquakeAttackAnim);
	AttackType = ETenebrisSpecialAttackType::None;
	SetSpecialAttackTimer(0);
}

bool ABossTenebris::FlyingEarthquakeAttackCondition()
{
	if (AttackType == ETenebrisSpecialAttackType::FlyingEarthquake)
	{
		return true;
	}

	return false;
}

void ABossTenebris::FlyingRightFireBallAttack()
{
	PlayAnimMontage(FlyingRightFireBallAttackAnim);
}

void ABossTenebris::MoveFrontLeft()
{
	PlayAnimMontage(MoveFrontLeftAnim);
}

void ABossTenebris::MoveFront()
{
	PlayAnimMontage(MoveFrontAnim);
}

void ABossTenebris::MoveBackLeft()
{
	PlayAnimMontage(MoveBackLeftAnim);
}

void ABossTenebris::MoveBackRight()
{
	PlayAnimMontage(MoveBackRightAnim);
}

void ABossTenebris::MoveBack()
{
	PlayAnimMontage(MoveBackAnim);
}

void ABossTenebris::Howling()
{
	PlayAnimMontage(HowlingAnim);
}

void ABossTenebris::SetSpecialAttackTimer(int8 index)
{
	GetWorldTimerManager().SetTimer(SpecialAttackTimer, FTimerDelegate::CreateLambda([&]
	{
		switch (index)
		{
		case 0:
			AttackType = ETenebrisSpecialAttackType::BressRight;
			break;
		case 1:
			AttackType = ETenebrisSpecialAttackType::FlyingEarthquake;
			break;
		default:
			break;
		}
	}), SpecialAttackInterval, false);
}