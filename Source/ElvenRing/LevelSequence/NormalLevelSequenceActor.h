#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "NormalLevelSequenceActor.generated.h"

UENUM(BlueprintType)
enum class ELevelPath : uint8
{
	None UMETA(DisplayName = "None"),
	Artorias UMETA(DisplayName = "Artorias"),
	Tenebris UMETA(DisplayName = "Tenebris"),
	HangedMan UMETA(DisplayName = "HangedMan"),
	Title UMETA(DisplayName = "Title")
};

class UElvenringGameInstance;
class ULevelSequence;
class ABoss;

UENUM(BlueprintType)
enum class ESequenceType : uint8
{
	Spawn,
	Phase,
	Dead
};

UCLASS()
class ELVENRING_API ANormalLevelSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 서버에서 시퀀스 시작 */
	UFUNCTION(BlueprintCallable)
	void StartSequence();

	/** 시퀀스 앤드 이벤트 핸들러 */
	UFUNCTION()
	void OnSequenceEnded();

	/** 시퀀스 종류별 처리 함수 */
	void OnSpawnSequenceEnded();
	void OnPhaseSequenceEnded();
	
	UFUNCTION(NetMulticast, Reliable)
	void OnDeadSequenceEnded();

private:
	/** 시퀀스 재생 멀티캐스트 함수 (모든 클라이언트에게 전파) */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySequence();
    
	/** 시퀀스 종료 멀티캐스트 함수 */
	UFUNCTION()
	void CheckCurrentSequenceType();
    
	/** 플레이어 가시성 제어 멀티캐스트 함수 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetAllPlayerHidden();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetAllPlayerUnhidden();
	
private:
	UPROPERTY(EditAnywhere, Category = "Boss")
	float SpawnSequenceDelegateDelay = 1.0f;
	
	UPROPERTY(VisibleAnywhere, Category = "Boss")
	ULevelSequence* CurrentLevelSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	ESequenceType SequenceType;

	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	ELevelPath NextLevelPath;

	UPROPERTY(VisibleAnywhere, Category = "Level Sequence")
	TMap<ELevelPath, FString> LevelPathMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Boss")
	float SceneTransitionDelay = 10.0f;

	UPROPERTY()
	ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY()
	UElvenringGameInstance* Instance;

	// 네트워크 동기화를 위한 복제 변수들
	UPROPERTY(Replicated)
	bool bIsSequencePlaying;
};
