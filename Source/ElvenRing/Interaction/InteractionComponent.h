﻿// Fill out your copyright notice in the Description page of Project Settings.

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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, const FString&, InteractText);
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
	/* 현재 선택된 Interact 객체와 상호작용을 진행한다. */
protected:
	/* Interactable을 탐색하는 간격 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SearchInterval;
	FTimerHandle SearchTimerHandle;
	/* 현재 상호작용 가능한 Actor */
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> CurrentInteractable;
	FString CurrentInteractText;

	/* 이전 프레임에서 Current Interactable 존재 여부, Weak Pointer로 저장하고 있는데 도중에 파괴된 것을 확인하기 위해 필요 */
	bool bHasTarget;
	
	/* 상호 작용 가능 대상을 선택할 때 거리 선호도를 결정한다. */
	UPROPERTY(EditDefaultsOnly)
	float DistanceWeight;
	/* 상호 작용 대상을 선택할 때 방향 선호도를 결정한다. 높을 수록 Actor의 Front 방향과 같은 방향 점수가 올라간다. */
	UPROPERTY(EditDefaultsOnly)
	float DirectionWeight;

	/** Interactable 검색을 해서 최신화 한다. */
	void SearchInteractable();
	/** 현재 위치에서 Interact 객체 검색 */
	void FindOverlapInteractables(TArray<FOverlapResult>& OverlapResults) const;
	/** Interact 객체들의 우선 순위 점수*/
	float CalculateMatchScore(const AActor* Interactable) const;
	virtual AActor* FindBestInteractable(const TArray<FOverlapResult>& OverlapResults);

public:
	/** 현재 선택된 Interatable과 상호작용을 실행한다. */
	UFUNCTION(BlueprintCallable)
	void PerformInteract();
	UFUNCTION(BlueprintCallable)
	bool HasInteractTarget() const;
	
protected:
	/** Authority Node에서 실질적으로 실행되는 Interaction */
	void HandleInteraction(AActor* InteractableActor);
	UFUNCTION(Server, Reliable)
	void ServerPerformInteract(AActor* InteractableActor);
	void ServerPerformInteract_Implementation(AActor* InteractableActor);
	/** Interactable 객체가 유효한지 판단한다.*/
	bool ValidateInteractable(AActor* InteractableActor) const;
protected:
	UFUNCTION()
	void LogInteractFound(const FString& InteractText);
	UFUNCTION()
	void LogInteractLost();
	bool IsOwnerLocallyControlled() const;
	// Called when the game starts
	virtual void BeginPlay() override;
};
