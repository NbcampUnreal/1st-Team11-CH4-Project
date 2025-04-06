// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainTitleWidget.generated.h"

class UButton;
class UTextBlock;
class USizeBox;
UCLASS()
class ELVENRING_API UMainTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable,Category = "UI")
	void OnClicked();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnPressed();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnHovered();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnUnhovered();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnReleased();
private:
	FTimerHandle AlphaPingpongTimerHandle;
	FTimerHandle OnClickTimerHandle;
	FTimerHandle OnPressTimerHandle;
	FTimerHandle OnHoverTimerHandle;
	FTimerHandle OnUnHoverTimerHandle;
	FTimerHandle OnReleaseTimerHandle;
	float time = 0.f;
	float Dir;
	void PingpongText();
	void HoverText();
	void UnhoverText();
	void OnReleaseText();
};
