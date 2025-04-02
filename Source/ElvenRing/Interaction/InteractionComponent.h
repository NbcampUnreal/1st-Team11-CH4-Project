// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

class IInteractable;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ELVENRING_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

public:
	/* log 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldLog;
	/* 상호작용 가능 거리, Sphere로 탐색한다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractRadius;
	/* Begin Play에서 자동으로 Search 실행 여부 */
	UPROPERTY(EditDefaultsOnly)
	bool bStartSearchOnBeginPlay;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, FString, InteractText);
	/* 상호작용 가능 Actor가 발견되었을 때 호출된다. */
	UPROPERTY(BlueprintAssignable)
	FOnInteractableFound OnInteractableFound;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnIteractableLost);
	/* 상호작용 가능 Actor가 사라졌을 때 호출된다. */
	UPROPERTY(BlueprintAssignable);
	FOnIteractableLost OnInteractableLost;

	/* Search를 시작한다. */
	UFUNCTION(BlueprintCallable)
	void StartSearch();
	/* Search를 중지한다. */
	UFUNCTION(BlueprintCallable)
	void StopSearch();

protected:
	/* Interactable을 탐색하는 간격 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SearchInterval;
	FTimerHandle SearchTimerHandle;
	/* 현재 상호작용 가능한 Actor */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> CurrentInteractable;

	/* 상호 작용 가능 대상을 선택할 때 거리 선호도를 결정한다. */
	UPROPERTY(EditDefaultsOnly)
	float DistanceWeight;
	/* 상호 작용 대상을 선택할 때 방향 선호도를 결정한다. 높을 수록 Actor의 Front 방향과 같은 방향 점수가 올라간다. */
	UPROPERTY(EditDefaultsOnly)
	float DirectionWeight;

	void SearchInteractable();
	float CalculateMatchScore(const AActor* Interactable) const;
	virtual AActor* FindBestInteractable(const TArray<FOverlapResult>& OverlapResults);
protected:
	UFUNCTION()
	void LogInteractFound(FString InteractText);
	UFUNCTION()
	void LogInteractLost();
	// Called when the game starts
	virtual void BeginPlay() override;
};
