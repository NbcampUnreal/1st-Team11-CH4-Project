// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainTitleWidget.generated.h"

UENUM(BlueprintType)
enum class EConnectState : uint8
{
	Ready = 0,
	Connecting = 1,
	ConnectionSuccessful = 2,
	ConnectionFailed = 3
};
struct FRamdaConnect
{
	float TargetValue = 0.f;
	float ValueMax = 0.f;
	float ElapsedTime = 0.f;
	float Duration = 1.f;
	float PrevTime = 0.f;
	float DelayTime = 1.f;

	FTimerHandle* DelayTimerHandle = nullptr;
	FTimerHandle* TimerHandle = nullptr;

	void ClearPointer()
	{
		//MyProgressBar = nullptr;
		//MyYellowProgressBar = nullptr;
	}
};

class UButton;
class UTextBlock;
class USizeBox;
UCLASS()
class ELVENRING_API UMainTitleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* SinglePlayButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiPlayButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextSinglePlay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextMultiPlay;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextConnection;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable,Category = "UI")
	void OnSingleButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnMultiButtonClicked();

private:
	FTimerHandle AlphaPingpongTimerHandle;
	FTimerHandle OnClickTimerHandle;

	float time = 0.f;
	float Dir;
	void PingpongText();

	EConnectState ConnecState = EConnectState::Ready;
	float ConnectingTime = 0.f;

	FTimerHandle TimerHandle;
	FTimerHandle DelayTimerHandle;

	void ConnetingTimer();
	void CloseConnectMaseege(FRamdaConnect& FRConnect);
};
