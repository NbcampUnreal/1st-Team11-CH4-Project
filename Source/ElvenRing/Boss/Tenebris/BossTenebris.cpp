#include "BossTenebris.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"

ABossTenebris::ABossTenebris()
{
}

void ABossTenebris::BeginPlay()
{
	LOG(TEXT("Begin!"));

	//NormalPattern->AddAttackPattern(this, &ABossTenebris::GrabAttack, FString("GrabAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::BressAttackFront, FString("BressAttackFront"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::BressAttackRight, FString("BressAttackRight"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::EnergyAttack, FString("EnergyAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::TailAttack, FString("TailAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::EarthquakeAttack, FString("EarthquakeAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::FlyingEarthquakeAttack, FString("FlyingEarthquakeAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::FlyingRightFireBallAttack, FString("FlyingRightFireBallAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::Howling, FString("Howling"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBackLeft, FString("MoveBackLeft"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBackRight, FString("MoveBackRight"));
	NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveBack, FString("MoveBack"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveFront, FString("MoveFront"));
	//NormalPattern->AddAttackPattern(this, &ABossTenebris::MoveFrontLeft, FString("MoveFrontLeft"));

	//NormalPattern->AddAttackPattern(this, &ABossTenebris::RushAttack, FString("RushAttack"));

	Super::BeginPlay();

	SetBossBattleMode();
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

void ABossTenebris::FlyingEarthquakeAttack()
{
	PlayAnimMontage(FlyingEarthquakeAttackAnim);
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
