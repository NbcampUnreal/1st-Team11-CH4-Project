// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainUi.generated.h"

class UProgressBar;
class UImage;
class AUnitBase;

struct FRamdaElement
{
	float TargetValue = 0.f;
	float ValueMax = 0.f;
	float ElapsedTime = 0.f;
	float Duration = 1.f;
	float PrevTime = 0.f;
	float DelayTime = 1.f;
	float CurProgressBarPer = 0.f;
	float TargetProgressBarPer = 0.f;
	
	FTimerHandle* DelayTimerHandle = nullptr;
	FTimerHandle* TimerHandle = nullptr;

	UProgressBar* MyProgressBar = nullptr;
	UProgressBar* MyYellowProgressBar = nullptr;
	void ClearPointer()
	{
		MyProgressBar = nullptr;
		MyYellowProgressBar = nullptr;
	}
};
struct FRamdaElement2
{
	float TargetValue = 0.f;
	float ValueMax = 0.f;
	float ElapsedTime = 0.f;
	float Duration = 1.f;
	float PrevTime = 0.f;
	float RecoverProgessBarPercent = 0.f;
	FVector2D CurrentSize ;
	FVector2D TargetSize ;
	FTimerHandle* DelayTimerHandle = nullptr;
	FTimerHandle* TimerHandle = nullptr;
	UProgressBar* MyProgressBar = nullptr;
	UProgressBar* TargetYellowProgressBar = nullptr;
	void ClearPointer()
	{
		MyProgressBar = nullptr;
		TargetYellowProgressBar = nullptr;
	}
};

UCLASS()
class ELVENRING_API UPlayerMainUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressYellowBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg3;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MpProgressBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MpProgressYellowBar;

	UPROPERTY(meta = (BindWidget))
	UImage* MpFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* MpFrameImg3;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StProgressBar;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StProgressYellowBar;

	UPROPERTY(meta = (BindWidget))
	UImage* StFrameImg2;
	UPROPERTY(meta = (BindWidget))
	UImage* StFrameImg3;

	TArray<UImage*> PortraitImages;

	UFUNCTION(BlueprintCallable,Category = "PlayerMainUi")
	void DecreaseHp(float TargetHp , float HpMax);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void DecreaseMp(float TargetMp, float MpMax);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void DecreaseSt(float TargetSt, float StMax);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void RecoverHp(float TargetHp, float HpMax, float RecoverSpeed = 1.5f);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void RecoverMp(float TargetMp, float MpMax, float RecoverSpeed = 1.5f);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void RecoverSt(float TargetSt, float StMax, float RecoverSpeed = 1.5f);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void LevelUp(float Hp, float St, float Mp);

	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void UpdateHp(float CurHp, float HpMax,int32 State);
	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void UpdateMp(float CurMp, float MpMax, int32 State);
	UFUNCTION(BlueprintCallable, Category = "PlayerMainUi")
	void UpdateSt(float CurSt, float StMax, int32 State);

	void BindToPlayer(AUnitBase* Player);
private:
	void UpdateFrameSize(UProgressBar* ProgressBar, UProgressBar* ProgressBar2, UImage* FrameImage2, UImage* FrameImage3, float LevelHp);
	void UpdateProgressBarSize(UProgressBar* ProgressBar, float XSize);

	void DecreaseProgressBar(FRamdaElement& FEmt);
	void RecoverProgressBar(FRamdaElement2& FEmt );
	void RecoverProgressBar1(FRamdaElement& FEmt);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	FTimerHandle HpTimerHandle;
	FTimerHandle StTimerHandle;
	FTimerHandle MpTimerHandle;

	FTimerHandle HpTimerDelayHandle;
	FTimerHandle StTimerDelayHandle;
	FTimerHandle MpTimerDelayHandle;
	
	float ElapsedTime1 = 0.f;
	float PrevTime1 = 0.f;
	
	float CurrentHp;
	float MaxHp;

	float CurrentMp;
	float MaxMp;

	float CurrentSt;
	float MaxSt;
};
