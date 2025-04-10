#include "BossTenebris.h"
#include "Components/CapsuleComponent.h"
#include "ElvenRing/ElvenRing.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Boss/BossPattern/BossSpecialPatternComponent.h"
#include "Kismet/GameplayStatics.h"

ABossTenebris::ABossTenebris()
{
	AttackType = ETenebrisSpecialAttackType::None;
	PhaseType = EPhaseType::One;

	SpecialAttackInterval = 40;

	// 보스 Bress 공격 판정 콜리전
	BressAttackCollision = CreateDefaultSubobject<UCapsuleComponent>("BressAttack Collision");
	BressAttackCollision->SetupAttachment(GetMesh());

	GrabAttackCollision = CreateDefaultSubobject<UCapsuleComponent>("GrabAttack Collision");
	GrabAttackCollision->SetupAttachment(GetMesh());
	
	UCapsuleComponent* TailAttackCollision1 = CreateDefaultSubobject<UCapsuleComponent>("TailAttack Collision1");
	TailAttackCollision1->SetupAttachment(GetMesh());

	UCapsuleComponent* TailAttackCollision2 = CreateDefaultSubobject<UCapsuleComponent>("TailAttack Collision2");
	TailAttackCollision2->SetupAttachment(GetMesh());

	TailAttackCollisions.Add(TailAttackCollision1);
	TailAttackCollisions.Add(TailAttackCollision2);
}

void ABossTenebris::ServerOnSpawnSequenceEnded_Implementation()
{
	Super::ServerOnSpawnSequenceEnded_Implementation();

	if (HasAuthority())
	{
		PlayAnimation(BressAfterMoveFrontAnim);
		GetWorldTimerManager().SetTimer(SpecialAttackTimer,FTimerDelegate::CreateLambda([&]
		{
			AttackType = ETenebrisSpecialAttackType::BressRight;
		}
		), SpecialAttackInterval, false);

		PlaySound(BossBattleBGM);
	}
}


void ABossTenebris::BeginPlay()
{
	NormalPattern->AddAttackPattern(this, &ABossTenebris::GrabAttack, FString("GrabAttack"));
	NormalPattern->AddAttackPattern(this, &ABossTenebris::TailAttack, FString("TailAttack"));
	NormalPattern->AddAttackPattern(this, &ABossTenebris::EarthquakeAttack, FString("EarthquakeAttack"));
	
	SpecialPattern->AddAttackPattern(this, &ABossTenebris::BressAttackRightCondition, &ABossTenebris::BressAttackRight, FString("BressAttackRight"));
	SpecialPattern->AddAttackPattern(this, &ABossTenebris::FlyingEarthquakeAttackCondition, &ABossTenebris::FlyingEarthquakeAttack, FString("FlyingEarthquakeAttack"));
	
	RegisterCollision(BressAttackCollision, BressCollisionSocketName);
	RegisterCollision(TailAttackCollisions[0], TailCollisionSocketName[0]);
	RegisterCollision(TailAttackCollisions[1], TailCollisionSocketName[1]);
	RegisterCollision(GrabAttackCollision, GrabCollisionSocketName);

	Super::BeginPlay();
}

void ABossTenebris::MulticastOnPhaseSequenceEnded_Implementation()
{
	Super::MulticastOnPhaseSequenceEnded();

	if (HasAuthority())
	{
		CurHealth = MaxHealth/2;
		PhaseType = EPhaseType::Two;
		SetAttackTarget();
		PlayAnimMontage(FlyingRightFireBallAttackAnim);
		GetWorldTimerManager().SetTimer(SpecialAttackTimer,FTimerDelegate::CreateLambda([&]
		{
			AttackType = ETenebrisSpecialAttackType::FlyingEarthquake;
		}
		), SpecialAttackInterval, false);

		PlaySound(BossBattleBGM2);
	}
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
			LOG(TEXT("Phase Two Begin"));
			UGameplayStatics::OpenLevel(this, FName("L_Tenebris2"));
		}
	}

	return Damage;
}

void ABossTenebris::GrabAttack()
{
	PlayAnimation(GrabAttackAnim);
}

void ABossTenebris::BressAttackFront()
{
	PlayAnimation(BressAttackFrontAnim);
}

void ABossTenebris::BressAttackRight()
{
	PlayAnimation(BressAttackRightAnim);
	AttackType = ETenebrisSpecialAttackType::None;
	GetWorldTimerManager().SetTimer(SpecialAttackTimer,FTimerDelegate::CreateLambda([&]
	{
		AttackType = ETenebrisSpecialAttackType::FlyingEarthquake;;
	}), SpecialAttackInterval, false);}

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
	PlayAnimation(EnergyAttackAnim);
}

void ABossTenebris::TailAttack()
{
	PlayAnimation(TailAttackAnim);
}

void ABossTenebris::EarthquakeAttack()
{
	PlayAnimation(EarthQuakeAttackAnim);
}

void ABossTenebris::RushAttack()
{
	PlayAnimation(RushAttackAnim);
}

void ABossTenebris::FlyingEarthquakeAttack()
{
	PlayAnimation(FlyingEarthquakeAttackAnim);
	AttackType = ETenebrisSpecialAttackType::None;
	GetWorldTimerManager().SetTimer(SpecialAttackTimer,FTimerDelegate::CreateLambda([&]
	{
		AttackType = ETenebrisSpecialAttackType::BressRight;;
	}), SpecialAttackInterval, false);
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
	PlayAnimation(FlyingRightFireBallAttackAnim);
}

void ABossTenebris::Howling()
{
	PlayAnimation(HowlingAnim);
}