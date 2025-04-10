// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScreenEffectWidget.generated.h"

struct FRamdaScreenEffect
{
	float ElapsedTime = 0.f;
	float Duration = 1.f;
	float PrevTime = 0.f;
	float DelayTime = 1.f;
	float StartValue = 0.f;
	float EndValue = 0.f;
	FTimerHandle* TimerHandle;
	FLinearColor Color;
};

class UImage;
UCLASS()
class ELVENRING_API UScreenEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* ImageScreen;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void FadeIn(float Time = 2.f, bool bAutoHideWhenFinished = false );//밝아짐
	UFUNCTION(BlueprintCallable, Category = "UI")
	void FadeOut(float Time = 2.f); //어두워짐
	UFUNCTION(BlueprintCallable, Category = "UI")
	void FadeOutIn(float FadeOut = 1.f, float DelayTime = 1.5f, float FadeIn = 1.f,bool bAutoHideWhenFinished = false );

private:
	bool bPlay;
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandle2;
};
