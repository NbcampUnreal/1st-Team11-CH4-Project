#pragma once

#include "CoreMinimal.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Sound/SoundBase.h"
#include "Containers/Map.h"
#include "NormalMonster.generated.h"


class UWidgetComponent; //ksw


UENUM(BlueprintType)
enum class ENormalMonsterSoundCategory : uint8
{
	MoveSound    UMETA(DisplayName = "Move"),
	AttackSound  UMETA(DisplayName = "Attack"),
	HitSound     UMETA(DisplayName = "Hit"),
	DeathSound   UMETA(DisplayName = "Death")
};

UCLASS()
class ELVENRING_API ANormalMonster : public AUnitBase
{
	GENERATED_BODY()

public:
	ANormalMonster();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Audio")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<USoundBase*> MoveSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<USoundBase*> AttackSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<USoundBase*> HitSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TArray<USoundBase*> DeathSounds;

	UFUNCTION(NetMulticast, Unreliable)
	void PlaySound(USoundBase*Sound);
	UFUNCTION(Server, Reliable)
	void PlayRandomSound(ENormalMonsterSoundCategory Category);

	
#pragma region 전투관련
	UFUNCTION(BlueprintCallable, Category="AI")
	virtual void Attack(AActor* Target) override;

	UFUNCTION(BlueprintCallable, Category="AI")
	void PlayerDetected(UObject* TargetObject);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	float AttackAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	bool MonsterIsHit;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	virtual void OnDeath() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void SpawnDecal(FVector HitLocation, FRotator HitRotation);
#pragma endregion
	
#pragma region 통신관련
	
	bool InstanceIsHit;
	bool InstanceIsAttack;
	bool InstanceIsDeath;
	
	UFUNCTION(Server, Reliable)
	void RPCIsHit(bool value);

	UFUNCTION(Server, Reliable)
	void RPCIsAttack(bool value);
	
	UFUNCTION(Server, Reliable)
	void RPCIsDeath(bool value);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsHit(bool value, FVector HitLocation, FRotator HitRotation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsAttack(bool value);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastIsDeath(bool value);
#pragma endregion

	void SetWidget(UUserWidget* Widget); //ksw
	void UpdateHPBar();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
	UWidgetComponent* HPWidgetComponent;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UMaterialInterface* DecalMaterial;

	FTimerHandle UpdateHPBarTimer;
	FTimerHandle StayTimer;
};
