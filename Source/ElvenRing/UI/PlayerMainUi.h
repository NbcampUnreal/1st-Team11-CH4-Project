// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainUi.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class ELVENRING_API UPlayerMainUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg3;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* MpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* MpFrameImg3;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StProgressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* StFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* StFrameImg3;

	TArray<UImage*> PortraitImages;

	UFUNCTION(BlueprintCallable,Category = "PlayerMainUi")
	void UpdateHpBar(float Hp);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void UpdateMpBar(float Mp);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void LevelUp(float Hp, float St, float Mp);
private:
	void FixFrameSize(UProgressBar* ProgressBar, UImage* FrameImage2, UImage* FrameImage3, float LevelHp);
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};
