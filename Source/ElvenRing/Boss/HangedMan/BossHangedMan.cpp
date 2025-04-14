#include "BossHangedMan.h"

#include "BossHangedManHeart.h"
#include "ElvenRing/ElvenRing.h"
#include "EngineUtils.h"
#include "ElvenRing/Boss/BossPattern/BossNormalPatternComponent.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/BossWidget.h"
#include "ElvenRing/UI/UIManager.h"

ABossHangedMan::ABossHangedMan()
{
	HealthPhase = EHealthPhase::None;
	LightningInterval = 0.5f;
	MaxLightningCount = 6;
	CurrentLightningCount = 0;
	LightningStartDelay = 1.5f;
	PlayerCount = 0;
}

void ABossHangedMan::BeginPlay()
{
	NormalPattern->AddAttackPattern(this, &ABossHangedMan::LightningAttack, FString("LightningAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossHangedMan::ElectricJavelinAttack, FString("ElectricJavelinAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossHangedMan::ThunderBoldAttack, FString("ThunderBoldAttack"));
	//NormalPattern->AddAttackPattern(this, &ABossHangedMan::ElectronicAttackUp, FString("ElectronicAttackUp"));
	//NormalPattern->AddAttackPattern(this, &ABossHangedMan::ElectronicAttackDown, FString("ElectronicAttackDown"));
	
	Super::BeginPlay();
}

void ABossHangedMan::OnDeath()
{
	Heart->Destroy();
	GetWorldTimerManager().ClearTimer(LightningTimer);
	Super::OnDeath();
}

void ABossHangedMan::OnSpawnSequenceEnded()
{
	Super::OnSpawnSequenceEnded();

	UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetWorld()->GetGameInstance());
	GameInstance->GetUIManager()->GetBossWidgetUi()->SetName("End Of The Abyss, Bizarre");
	if (HasAuthority())
	{
		SetBossBattleMode_Implementation();
		PlaySound(BossBattleBGM);
	}
}

float ABossHangedMan::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CheckHealthPhase();
	return Damage;
}

void ABossHangedMan::CheckHealthPhase()
{
	if (CurHealth <= MaxHealth/4*3 && HealthPhase == EHealthPhase::None)
	{
		HealthPhase = EHealthPhase::First;
		Groggy();
	}
	else if (CurHealth <= MaxHealth/2 && HealthPhase == EHealthPhase::First)
	{
		HealthPhase = EHealthPhase::Second;
		SpecialAttack();
	}
	else if (CurHealth <= MaxHealth/4 && HealthPhase == EHealthPhase::Second)
	{
		HealthPhase = EHealthPhase::Third;
		Groggy();
	}
}

void ABossHangedMan::LightningAttack()
{
	PlayAnimation(LightningAnim);

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(LightningTimer, this, &ABossHangedMan::GetAllPlayersLocation, LightningInterval, true, LightningStartDelay);
	}
}

void ABossHangedMan::MulticastLightning_Implementation()
{
	for (int8 i=0; i<PlayerCount; ++i)
	{
		FVector SpawnLocation = FVector( PlayersLocation[i].X, PlayersLocation[i].Y, -2100.f );
		GetWorld()->SpawnActor<AActor>(LightningNiagara, SpawnLocation, FRotator::ZeroRotator);
	}

	++CurrentLightningCount;
	
	if (CurrentLightningCount >= MaxLightningCount)
	{
		CurrentLightningCount = 0;
		GetWorldTimerManager().ClearTimer(LightningTimer);
	}
}

void ABossHangedMan::ElectricJavelinAttack()
{
	PlayAnimation(ElectricJavelinAnim);
}

void ABossHangedMan::ThunderBoldAttack()
{
	PlayAnimation(ThunderBoldAnim);
}

void ABossHangedMan::ElectronicAttackUp()
{
	PlayAnimation(ElectronicUpAnim);
}

void ABossHangedMan::ElectronicAttackDown()
{
	PlayAnimation(ElectronicDownAnim);
}

void ABossHangedMan::SpecialAttack()
{
	MulticastStopBattleMode();
	PlayAnimation(SpecialAttackAnim);
}

void ABossHangedMan::Groggy()
{
	MulticastStopBattleMode();
	PlayAnimation(GroggyAnim);
}

void ABossHangedMan::GetAllPlayersLocation()
{
	PlayersLocation.Empty();
	PlayerCount = 0;
	
	for (AElvenRingCharacter* Player : TActorRange<AElvenRingCharacter>(GetWorld()))
	{
		if (IsValid(Player))
		{
			PlayersLocation.Add(Player->GetActorLocation());
			++PlayerCount;
		}
	}

	FTimerHandle MulticastLightningTimer;
	GetWorldTimerManager().SetTimer(MulticastLightningTimer, this,
		&ABossHangedMan::MulticastLightning_Implementation, LightningInterval, false, true);
}