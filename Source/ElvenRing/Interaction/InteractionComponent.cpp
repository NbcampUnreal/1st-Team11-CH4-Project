// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "Interactable.h"
#include "Engine/OverlapResult.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bShouldLog = false;
	InteractRadius = 200.0f;
	bStartSearchOnBeginPlay = true;
	SearchInterval = 0.1f;

	bHasTarget = false;
	
	DistanceWeight = 0.7f;
	DirectionWeight = 0.3f;

	SetIsReplicatedByDefault(true);
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
	FindOverlapInteractables(OverlapResults);

	AActor* NewInteractable = FindBestInteractable(OverlapResults);
	AActor* OldInteractable = CurrentInteractable.Get();

	// Actor가 프레임 사이에서 파괴되었을 경우
	if (bHasTarget && !CurrentInteractable.IsValid())
	{
		OnInteractableLost.Broadcast();
	}

	if (OldInteractable != NewInteractable)
	{
		if (OldInteractable)
		{
			OnInteractableLost.Broadcast();
		}

		CurrentInteractable = NewInteractable;
		if (NewInteractable)
		{
			if (IInteractable* Interactable = Cast<IInteractable>(NewInteractable))
			{
				FString InteractText = Interactable->GetInteractText();
				OnInteractableFound.Broadcast(InteractText);
			}
		}
	}

	bHasTarget = CurrentInteractable.IsValid();
}

void UInteractionComponent::FindOverlapInteractables(TArray<FOverlapResult>& OverlapResults) const
{
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(InteractRadius);
	GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetOwner()->GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape
	);
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

void UInteractionComponent::PerformInteract()
{
	if (!CurrentInteractable.IsValid())
	{
		return;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		HandleInteraction(CurrentInteractable.Get());
	}
	else
	{
		ServerPerformInteract(CurrentInteractable.Get());
	}
}

bool UInteractionComponent::HasInteractTarget() const
{
	return CurrentInteractable.IsValid();
}

void UInteractionComponent::HandleInteraction(AActor* InteractableActor)
{
	if (GetOwnerRole() != ROLE_Authority) return;
	if (!ValidateInteractable(InteractableActor)) return;

	if (IInteractable* Interactable = Cast<IInteractable>(InteractableActor))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
		{
			Interactable->Interact(PlayerController);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractableActor is not IInteractable"));
	}
}

void UInteractionComponent::ServerPerformInteract_Implementation(AActor* InteractableActor)
{
	HandleInteraction(InteractableActor);
}

bool UInteractionComponent::ValidateInteractable(AActor* InteractableActor) const
{
	// 1. 상호 작용 객체가 Server에 아직 존재하는지 판단
	if (!InteractableActor || !GetOwner())
	{
		return false;
	}

	// 2. 상호 작용 객체가 거리 이내에 존재하는지 판단
	TArray<FOverlapResult> OverlapResults;
	FindOverlapInteractables(OverlapResults);
	const bool bOverlap = OverlapResults.ContainsByPredicate([InteractableActor](const FOverlapResult& Result)
	{
		return Result.GetActor() == InteractableActor;
	});
	if (!bOverlap)
	{
		return false;
	}

	// 3. Can Interact 확인
	IInteractable* Interactable = Cast<IInteractable>(InteractableActor);
	return Interactable && Interactable->CanInteract();
}

void UInteractionComponent::LogInteractFound(FString InteractText)
{
	UE_LOG(LogTemp, Display, TEXT("Interactable Found: %s"), *InteractText);
}

void UInteractionComponent::LogInteractLost()
{
	UE_LOG(LogTemp, Display, TEXT("Interactable Lost"));
}

bool UInteractionComponent::IsOwnerLocallyControlled() const
{
	if (APawn* Owner = Cast<APawn>(GetOwner()))
	{
		return Owner->IsLocallyControlled();
	}
	return false;
}

float UInteractionComponent::CalculateMatchScore(const AActor* Interactable) const
{
	// 거리와 방향 2가지 기준이 존재한다.
	// 거리는 가까울 수록 높은 점수가 높다.
	// 방향은 Actor Forward에 일치할 수록 점수가 높다.
	float Distance = (FVector::Distance(GetOwner()->GetActorLocation(), Interactable->GetActorLocation()));
	float DistanceScore = 1 - Distance / InteractRadius; // 0.0f ~ 1.0f
	// Overlap으로 판별하기 때문에 Actor 중심에서의 거리가 Interact Radius보다 커질 수 있다.
	// Direction Weight와 곱해지기 때문에 최소한 작은 값을 넣는다.
	DistanceScore = FMath::Clamp(DistanceScore, 0.01f, 1.0f); 
	
	FVector ToTarget = Interactable->GetActorLocation() - GetOwner()->GetActorLocation();
	ToTarget.Normalize();
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
