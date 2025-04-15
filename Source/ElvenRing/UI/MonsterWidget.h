// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class UCanvasPanel;
class AUnitBase;

struct FMRamdaElement
{
	bool Recover = false;
	FText DamageTextValue;
	float TargetHp = 0.f;
	float HpMax = 0.f;
	float CurProgressBarPer = 0.f;
	float TargetProgressBarPer = 0.f;
	float ElapsedTime = 0.f;
	float Duration = 1.f;
	float DelayTime = 0.1f;
	float PrevTime = 0.f;
	FTimerHandle* HpTimerDelayHandle;
	FTimerHandle* HpTimerHandle;
	UProgressBar* MyProgressBar = nullptr;
	UProgressBar* MyProgressYellowBar = nullptr;
	void ClearPointer()
	{
		MyProgressBar = nullptr;
		MyProgressYellowBar = nullptr;
	}
};

UCLASS()
class ELVENRING_API UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanelSlot;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressYellowBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector2D UIScale  = FVector2D(1.0f, 1.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
	FVector2D	UIPos = FVector2D::ZeroVector;

	UFUNCTION(BlueprintCallable, Category =  "UI")
	void SetUiSize(FVector2D  Scale, FVector2D Pos);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DecreaseHp(float TargetHp, float HpMax);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RecoverHp(float TargetHp, float HpMax);

	UFUNCTION()
	void UpdateHp(float TargetHp, float HpMax, int32 State);

	void BindToMonster(AUnitBase* Monster);

	UFUNCTION(BlueprintCallable, Category = "Ui")
	void SetActiveWidget(bool bShow);
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
private:
	FTimerHandle HpTimerHandle;
	FTimerHandle HpTimerDelayHandle;
	AUnitBase* TempMonster = nullptr;
	void UpdateProgressBar(FMRamdaElement& FElement);
};
