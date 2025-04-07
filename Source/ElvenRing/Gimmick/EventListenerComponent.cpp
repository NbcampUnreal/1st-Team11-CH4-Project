// Fill out your copyright notice in the Description page of Project Settings.


#include "EventListenerComponent.h"

#include "EventManager.h"
#include "GameFlag.h"
#include "Algo/AllOf.h"
#include "Algo/AnyOf.h"
#include "ElvenRing/Core/ElvenRingGameMode.h"


bool FRequirement::Evaluate(const UEventManager* Evaluator) const
{
	if (ConditionType == EConditionType::All)
	{
		return Algo::AllOf(Conditions, [&Evaluator](const FCondition& Condition)
		{
			if (const bool Value = Evaluator->GetGameFlag(Condition.TargetGameFlag->FlagName))
			{
				return Value == Condition.bExpectedValue;
			}
			return false;
		});
	}
	else if (ConditionType == EConditionType::Any)
	{
		return Algo::AnyOf(Conditions, [&Evaluator](const FCondition& Condition)
		{
			if (const bool Value = Evaluator->GetGameFlag(Condition.TargetGameFlag->FlagName))
			{
				return Value == Condition.bExpectedValue;
			}
			return false;
		});
	}
	return false;
}

void FRequirement::UpdateCondition(const class UEventManager* EventEvaluator)
{
	IsMet = Evaluate(EventEvaluator);
}

// Sets default values for this component's properties
UEventListenerComponent::UEventListenerComponent(): Requirement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UEventListenerComponent::OnFlagChanged(bool bNewValue, bool bOldValue)
{
	if (Requirement.IsMet)
	{
		return;
	}
	if (AElvenRingGameMode* GameMode = Cast<AElvenRingGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (UEventManager* EventManager = GameMode->GetEventManager())
		{
			Requirement.UpdateCondition(EventManager);
			if (Requirement.IsMet)
			{
				OnEventTriggered.Broadcast();
			}
		}
	}
}


// Called when the game starts
void UEventListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Game Logic은 서버에서만 작동한다.
	if (GetOwner()->GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	AElvenRingGameMode* GameMode = Cast<AElvenRingGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		return;
	}

	UEventManager* EventManager = GameMode->GetEventManager();
	for (const FCondition& Condition : Requirement.Conditions)
	{
		if (UGameFlag* Flag = Condition.TargetGameFlag)
		{
			if (FOnFlagChanged* Delegate = EventManager->GetOnFlagChanged(Flag->FlagName))
			{
				Delegate->AddDynamic(this, &UEventListenerComponent::OnFlagChanged);
			}
		}
	}

	Requirement.UpdateCondition(EventManager);
}
