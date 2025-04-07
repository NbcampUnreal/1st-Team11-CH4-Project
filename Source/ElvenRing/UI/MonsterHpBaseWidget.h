// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHpBaseWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class UCanvasPanel;
struct FMRamdaElement;
//struct FMRamdaElement
//{
//	bool Recover = false;
//	FText DamageTextValue;
//	float TargetHp = 0.f;
//	float HpMax = 0.f;
//	float CurProgressBarPer = 0.f;
//	float TargetProgressBarPer = 0.f;
//	float ElapsedTime = 0.f;
//	float Duration = 1.f;
//	float PrevTime = 0.f;
//	UProgressBar* MyProgressBar = nullptr;
//	UProgressBar* MyProgressYellowBar = nullptr;
//	void ClearPointer()
//	{
//		MyProgressBar = nullptr;
//		MyProgressYellowBar = nullptr;
//	}
//};

UCLASS()
class ELVENRING_API UMonsterHpBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMonsterHpBaseWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void SetUiSize(FVector2D  Scale, FVector2D Pos);

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void DecreaseHp(float CurHp, float HpMax);

	UFUNCTION(BlueprintCallable, Category = "UI")
	virtual void RecoverHp(float TargetHp, float HpMax);

protected:

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


	virtual void UpdateProgressBar(FMRamdaElement& FElement);

	FTimerHandle HpTimerHandle;
	FTimerHandle HpTimerDelayHandle;
};
