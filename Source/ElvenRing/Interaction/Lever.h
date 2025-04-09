// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UCLASS()
class ELVENRING_API ALever : public AActor, public IInteractable
{
public:
	// IInteractable Interface
	virtual FString GetInteractText() override;
	virtual void Interact(APlayerController* Player) override;
	virtual bool CanInteract() override;
	// IInteractable Interface

private:
	GENERATED_BODY()

public:
	ALever();

protected:
	virtual void BeginPlay() override;

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeverStateChanged, bool, bIsLeverOn);
	FOnLeverStateChanged OnLeverStateChanged;

	/** Lock이 되어 있으면 변경이 불가능하다.*/
	UFUNCTION(BlueprintCallable)
	void TryChangeLeverState(bool bNewLeverOn);
	UFUNCTION(BlueprintCallable)
	void SetLeverLock(bool bNewLocked);
	UFUNCTION(BlueprintCallable)
	void ToggleState();
protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnLeverOn"))
	void K2_OnLeverOn();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnLeverOff"))
	void K2_OnLeverOff();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsLeverOn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanLeverOn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanLeverOff;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsLocked;
};
