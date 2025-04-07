// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "FlagTrigger.generated.h"

UCLASS()
class ELVENRING_API AFlagTrigger : public AActor, public IInteractable
{
public:
	virtual FString GetInteractText() override;
	virtual void Interact(APlayerController* Player) override;
	virtual bool CanInteract() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlagTrigger();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEventTriggered);
	UPROPERTY(BlueprintAssignable)
	FOnEventTriggered OnEventTriggered;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class UGameFlag* GameFlagAsset;
	UPROPERTY(EditAnywhere)
	bool TargetCondition;
};
