// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingPlayerState.h"

#include "ElvenRing/Character/ElvenRingCharacter.h"

void FStatusSaved::SaveStatus(class AElvenRingCharacter* Character)
{
	if (!Character)
	{
		return;
	}

	CurHealth = Character->GetCurHealth();
	CurStamina = Character->GetCurStamina();
	CurMana = Character->GetCurMana();
}

void FStatusSaved::LodStatus(class AElvenRingCharacter* Character) const
{
	Character->SetCurrentHealth(CurHealth);
	Character->SetCurrentStamina(CurStamina);
	Character->SetCurrentMana(CurMana);
}

AElvenRingPlayerState::AElvenRingPlayerState()
{
	bHasSaved = false;
}

void AElvenRingPlayerState::SaveCharacterStatus(AElvenRingCharacter* Character)
{
	StatusSaved.SaveStatus(Character);
	bHasSaved = true;
}

void AElvenRingPlayerState::LoadCharacterStatus(class AElvenRingCharacter* Character)
{
	StatusSaved.LodStatus(Character);
}

void AElvenRingPlayerState::RecordPlayerDamage(AActor* DamagedActor, float Damage)
{
	if (!DamagedActor || Damage < 0.f)
	{
		return;
	}

	float& TotalDamage = BossDamageRecord.FindOrAdd(GetNativeClass(DamagedActor), 0.f);
	TotalDamage += Damage;

	TotalDamageDealt += Damage;
	
	UE_LOG(LogTemp,Display, TEXT("Player ID: %s, Damage Recorded: %s -> %f"), *GetName(), *GetNativeClass(DamagedActor)->GetName(), TotalDamage);
}

void AElvenRingPlayerState::RecordPlayerDamageTaken(AActor* DamagedActor, float Damage)
{
	if (!DamagedActor || Damage < 0.f)
	{
		return;
	}

	TotalDamageTaken += Damage;

	UE_LOG(LogTemp,Display,TEXT("Player ID: %s, Damage Taken: %s -> %f"), *GetName(), *GetNativeClass(DamagedActor)->GetName(), TotalDamageTaken);
}

void AElvenRingPlayerState::RecordDodge(AUnitBase* DamagedActor, float Damage)
{
	TotalDodgeCount++;
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
		OtherPlayerState->StatusSaved = StatusSaved;
		OtherPlayerState->bHasSaved = bHasSaved;
		OtherPlayerState->TotalDamageDealt = TotalDamageDealt;
		OtherPlayerState->TotalDamageTaken = TotalDamageTaken;
		OtherPlayerState->TotalDodgeCount = TotalDodgeCount;
		OtherPlayerState->RespawnCount = RespawnCount;
	};
}
