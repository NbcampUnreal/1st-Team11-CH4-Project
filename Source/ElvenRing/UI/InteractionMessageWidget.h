// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageWidgetBase.h"
#include "ElvenRing/Character/UnitBase.h"
#include "InteractionMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API UInteractionMessageWidget : public UMessageWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void ShowMessageText_Implementation(const FString& Message) override;
	virtual void SetActiveWidget(bool bActive) override;
	UFUNCTION()
	void HideMessageText();
	void BindToPlayer(AUnitBase* Unit);

protected:
	
	// Forward : Fade In, Reverse : Fade Out
	bool bIsForwarded = false;
	bool bIsActive = true;
	UFUNCTION()
	void OnFadeAnimationEnd();
	void PlayFadeInAnimation();
	void PlayFadeOutAnimation();
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
