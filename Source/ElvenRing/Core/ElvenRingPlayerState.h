// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "GameFramework/PlayerState.h"
#include "ElvenRingPlayerState.generated.h"

USTRUCT()
struct FStatusSaved	
{
	GENERATED_BODY()

	float MaxHealth;
	UPROPERTY(VisibleInstanceOnly)
	float CurHealth;
	float MaxStamina;
	float CurStamina;
	float MaxMana;
	float CurMana;
	float AttackPower;

	void SaveStatus(class AElvenRingCharacter* Character);
	void LodStatus(class AElvenRingCharacter* Character) const;
};

/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AElvenRingPlayerState();

public:
	void SaveCharacterStatus(class AElvenRingCharacter* Character);
	void LoadCharacterStatus(class AElvenRingCharacter* Character);
	bool HasSaved() const { return bHasSaved; }
protected:
	UPROPERTY(VisibleInstanceOnly)
	FStatusSaved StatusSaved;
	bool bHasSaved;

public:
	void RecordPlayerDamage(AActor* DamagedActor, float Damage);
	void RecordPlayerDamageTaken(AActor* DamagedActor, float Damage);
	void RecordDodge(AUnitBase* DamagedActor, float Damage);
	float GetBossDamage(const AActor* BossActor) const;
	void ResetPlayerDamage(AActor* BossClass);
protected:
	static UClass* GetNativeClass(const AActor* Actor);
	TMap<UClass*, float> BossDamageRecord;
	float TotalDamageDealt;
	float TotalDamageTaken;
	float TotalDodgeCount;
	int RespawnCount;

protected:
	virtual void BeginPlay() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
};
