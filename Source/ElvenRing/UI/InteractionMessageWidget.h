// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessageWidgetBase.h"
#include "InteractionMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API UInteractionMessageWidget : public UMessageWidgetBase
{
	GENERATED_BODY()

	virtual void ShowMessageText_Implementation(const FString& Message) override;
	void HideMessageText();

protected:
	// Forward : Fade In, Reverse : Fade Out
	bool bIsForwarded;
	UFUNCTION()
	void OnFadeAnimationEnd();
	void PlayFadeInAnimation();
	void PlayFadeOutAnimation();
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
