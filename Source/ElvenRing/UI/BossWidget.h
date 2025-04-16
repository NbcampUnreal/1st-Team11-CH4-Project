// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterHpBaseWidget.h" 

#include "BossWidget.generated.h"

class AUnitBase;

UCLASS()
class ELVENRING_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetUiSize(FVector2D  Scale, FVector2D Pos);
	UFUNCTION()
	void UpdateHp(float TargetHp, float HpMax, int32 State);
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
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg1;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector2D UIScale = FVector2D(1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector2D	UIPos = FVector2D::ZeroVector;

	void BindToBoss(AUnitBase* Boss);

	UFUNCTION(BlueprintCallable, Category = "Ui")
	void SetName(FString Name);

	UFUNCTION(BlueprintCallable, Category = "Ui")
	void SetActiveWidget(bool bShow);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	void UpdateProgressBar(FMRamdaElement& FElement);

	

	FTimerHandle HpTimerHandle;
	FTimerHandle HpTimerDelayHandle;
};
