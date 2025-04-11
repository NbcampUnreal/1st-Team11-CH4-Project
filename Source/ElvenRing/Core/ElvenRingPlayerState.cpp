// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingPlayerState.h"

AElvenRingPlayerState::AElvenRingPlayerState()
{
}

void AElvenRingPlayerState::RecordPlayerDamage(AActor* DamagedActor, float Damage)
{
	if (!DamagedActor || Damage < 0.f)
	{
		return;
	}

	float& TotalDamage = BossDamageRecord.FindOrAdd(GetNativeClass(DamagedActor), 0.f);
	TotalDamage += Damage;

	UE_LOG(LogTemp,Display, TEXT("Player ID: %s, Damage Recorded: %s -> %f"), *GetName(), *GetNativeClass(DamagedActor)->GetName(), TotalDamage);
}

float AElvenRingPlayerState::GetBossDamage(const AActor* BossActor) const
{
	if (!BossActor)
	{
		return 0.f;
	}

	if (const float* DamageValue = BossDamageRecord.Find(GetNativeClass(BossActor)))
	{
		return *DamageValue;
	}
	else
	{
		return 0.f;
	}
}

void AElvenRingPlayerState::ResetPlayerDamage(AActor* BossClass)
{
	if (!BossClass)
	{
		return;
	}

	UClass* BossNativeClass = GetNativeClass(BossClass);
	if (BossDamageRecord.Contains(BossNativeClass))
	{
		BossDamageRecord.Remove(BossNativeClass);
	}
}

UClass* AElvenRingPlayerState::GetNativeClass(const AActor* Actor)
{
	if (!Actor)
	{
		return  nullptr;
	}

	UClass* ActorClass = Actor->GetClass();
	while (ActorClass && !ActorClass->IsNative())
	{
		ActorClass = ActorClass->GetSuperClass();
	}

	return  ActorClass;
}

void AElvenRingPlayerState::BeginPlay()
{
	Super::BeginPlay();

}

void AElvenRingPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (AElvenRingPlayerState* OtherPlayerState = Cast<AElvenRingPlayerState>(PlayerState))
	{
		OtherPlayerState->BossDamageRecord = BossDamageRecord;
	};
}
