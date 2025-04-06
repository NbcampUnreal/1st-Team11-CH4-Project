// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterHpBaseWidget.h" 
#include "BossWidget.generated.h"


UCLASS()
class ELVENRING_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUiSize(FVector2D  Scale, FVector2D Pos);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DecreaseHp(float TargetHp, float HpMax);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RecoverHp(float TargetHp, float HpMax);


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanelSlot;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressYellowBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg1;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg3;

private:
	void UpdateProgressBar(FMRamdaElement& FElement);

	FTimerHandle HpTimerHandle;
	FTimerHandle HpTimerDelayHandle;
};
