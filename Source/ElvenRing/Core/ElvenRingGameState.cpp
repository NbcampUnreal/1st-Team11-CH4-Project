// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingGameState.h"

#include "ElvenRingPlayerState.h"

AElvenRingGameState::AElvenRingGameState()
{
	bActorSeamlessTraveled = true;
}

void AElvenRingGameState::BeginPlay()
{
	Super::BeginPlay();
}

void AElvenRingGameState::RecordPlayerDamage(APlayerController* EventInstigator, AActor* DamagedActor, float Damage)
{
	if (!EventInstigator || !EventInstigator->PlayerState || !DamagedActor || Damage < 0.f)
	{
		return;
	}
	
	AElvenRingPlayerState* PlayerState = EventInstigator->GetPlayerState<AElvenRingPlayerState>();
	FUniqueNetIdPtr PlayerIdPtr = PlayerState->GetUniqueId().GetUniqueNetId();
	UE_LOG(LogTemp,Display, TEXT("Player ID: %s"), *PlayerIdPtr->ToString());
	if (!PlayerIdPtr.IsValid())
	{
		UE_LOG(LogTemp,Display, TEXT("Invalid Player ID"));
		return;
	}

	TMap<UClass*, float>& DamageRecord = PlayerBossDamageRecord.FindOrAdd(PlayerIdPtr.ToSharedRef());
	UClass* BossNativeClass = GetNativeClass(DamagedActor);
	float& DamageValue = DamageRecord.FindOrAdd(BossNativeClass, 0.f);
	DamageValue += Damage;

	PlayerState->ApplyBossDamage(Damage);

	UE_LOG(LogTemp, Display, TEXT("Damage Recorded: %s -> %s, Damage: %f"), *EventInstigator->GetName(), *BossNativeClass->GetName(), DamageValue);
}

UClass* AElvenRingGameState::GetNativeClass(const AActor* Actor)
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

APlayerController* AElvenRingGameState::GetHighestDamagePlayer(const AActor* BossActor) const
{
	if (!BossActor)
	{
		return  nullptr;
	}

	UClass* BossNativeClass = GetNativeClass(BossActor);
	APlayerController* HighestDamagePlayer = nullptr;
	float HighestDamage = 0.f;
		
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC || !PC->PlayerState)
		{
			continue;
		}

		FUniqueNetIdPtr PlayerIdPtr = PC->PlayerState->GetUniqueId().GetUniqueNetId();
		const TMap<UClass*, float>* DamageRecord = PlayerBossDamageRecord.Find(PlayerIdPtr.ToSharedRef());
		if (!DamageRecord)
		{
			continue;
		}

		if (const float* DamageValue = DamageRecord->Find(BossNativeClass); DamageValue && *DamageValue > HighestDamage)
		{
			HighestDamage = *DamageValue;
			HighestDamagePlayer = PC;
		}
	}

	return HighestDamagePlayer;
}

void AElvenRingGameState::ResetDamageRecord(const UClass* BossClass)
{
	for (auto Elem : PlayerBossDamageRecord)
	{
		TMap<UClass*, float>& DamageRecord = Elem.Value;
		if (DamageRecord.Contains(BossClass))
		{
			DamageRecord.Remove(BossClass);
		}
	}
}