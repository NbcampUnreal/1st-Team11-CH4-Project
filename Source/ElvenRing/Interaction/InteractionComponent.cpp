// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Interactable.h"
#include "Chaos/AABBTree.h"
#include "Engine/OverlapResult.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bShouldLog = false;
	InteractRadius = 200.0f;
	bStartSearchOnBeginPlay = true;
	SearchInterval = 0.1f;

	DistanceWeight = 0.7f;
	DirectionWeight = 0.3f;
}


void UInteractionComponent::StartSearch()
{
	GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &UInteractionComponent::SearchInteractable, SearchInterval, true);
}

void UInteractionComponent::StopSearch()
{
    if (GetWorld()->GetTimerManager().IsTimerActive(SearchTimerHandle))
    {
        GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
    }
}

void UInteractionComponent::SearchInteractable()
{
	if (InteractRadius <= 0.0f) return;
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractRadius);
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetOwner()->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape
	);

	if (AActor* BestInteractable = FindBestInteractable(OverlapResults))
	{
		if (CurrentInteractable.IsValid() && CurrentInteractable.Get() != BestInteractable)
		{
			OnInteractableLost.Broadcast();
		}
		
		CurrentInteractable = BestInteractable;

		IInteractable* Interactable = Cast<IInteractable>(CurrentInteractable.Get());
		OnInteractableFound.Broadcast(Interactable->GetInteractText());
	}
	else
	{
		if (CurrentInteractable.IsValid())
		{
			OnInteractableLost.Broadcast();
			CurrentInteractable = nullptr;
		}
	}
}

AActor* UInteractionComponent::FindBestInteractable(const TArray<FOverlapResult>& OverlapResults)
{
	AActor* BestInteractable = nullptr;
	float BestScore = -1.0f;

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* Actor = Result.GetActor();
		IInteractable* Interactable = Cast<IInteractable>(Actor);
		if (Interactable && Interactable->CanInteract())
		{
			float Score = CalculateMatchScore(Result.GetActor());
			if (Score > BestScore)
			{
				BestScore = Score;
				BestInteractable = Actor;
			}
		}
	}

	return BestInteractable;
}

void UInteractionComponent::LogInteractFound(FString InteractText)
{
	UE_LOG(LogTemp, Display, TEXT("Interactable Found: %s"), *InteractText);
}

void UInteractionComponent::LogInteractLost()
{
	UE_LOG(LogTemp, Display, TEXT("Interactable Lost"));
}

float UInteractionComponent::CalculateMatchScore(const AActor* Interactable) const
{
	// 거리와 방향 2가지 기준이 존재한다.
	// 거리는 가까울 수록 높은 점수가 높다.
	// 방향은 Actor Forward에 일치할 수록 점수가 높다.
	float Distance = (FVector::Distance(GetOwner()->GetActorLocation(), Interactable->GetActorLocation()));
	float DistanceScore = 1 - InteractRadius / Distance; // 0.0f ~ 1.0f

	FVector ToTarget = Interactable->GetActorLocation() - GetOwner()->GetActorLocation();
	float Direction = FVector::DotProduct(GetOwner()->GetActorForwardVector(), ToTarget); // -1.0f ~ 1.0f
	float DirectionScore = (Direction + 1.0f) * 0.5f; // 0.0f ~ 1.0f

	return (DistanceScore * DistanceWeight) * (DirectionScore * DirectionWeight);
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bStartSearchOnBeginPlay)
	{
		StartSearch();
	}

	if (bShouldLog)
	{
		OnInteractableFound.AddDynamic(this, &UInteractionComponent::LogInteractFound);
		OnInteractableLost.AddDynamic(this, &UInteractionComponent::LogInteractLost);
	}
}
