// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidgetBase.generated.h"

class UTextBlock;

UCLASS()
class ELVENRING_API UMessageWidgetBase : public UUserWidget 
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextMessage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeAnim;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "UI")
	void ShowMessageText(const FString& Message);
	

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void SetActiveWidget(bool bActive);
	
protected:
	bool bCollapsed;
	FWidgetAnimationDynamicEvent OnFadeFinishedDelegate;

	void PlayFadeAnimation();

	UFUNCTION()
	void OnFadeAnimationFinished();
};
