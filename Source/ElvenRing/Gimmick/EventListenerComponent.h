// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EventListenerComponent.generated.h"

UENUM()
enum class EConditionType : UINT8
{
	All,
	Any,
};

USTRUCT(Blueprintable)
struct FCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UGameFlag* TargetGameFlag;
	UPROPERTY(EditAnywhere)
	bool bExpectedValue;
};

USTRUCT(Blueprintable)
struct FRequirement
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TArray<FCondition> Conditions;
	UPROPERTY(EditAnywhere)
	EConditionType ConditionType;

	bool Evaluate(const class UEventManager* EventEvaluator) const;
	void UpdateCondition(const class UEventManager* EventEvaluator);
	bool IsMet = false;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ELVENRING_API UEventListenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEventListenerComponent();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEventTriggered);
	UPROPERTY(BlueprintAssignable)
	FOnEventTriggered OnEventTriggered;
	
protected:
	UFUNCTION()
	void OnFlagChanged(bool bNewValue, bool bOldValue);
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FRequirement Requirement;
};
