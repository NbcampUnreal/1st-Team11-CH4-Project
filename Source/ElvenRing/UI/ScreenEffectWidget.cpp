// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenEffectWidget.h"
#include "Components/Image.h"

void UScreenEffectWidget:: FadeIn(float Time, bool bAutoHideWhenFinished )
{
	if (bPlay)
		return;
	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	bPlay = true;
	FLinearColor CurrentColor = ImageScreen->ColorAndOpacity;
	CurrentColor.A = 1.f;
	ImageScreen->SetColorAndOpacity(CurrentColor);

	FRamdaScreenEffect RamdaElement;
	RamdaElement.Color = CurrentColor;
	RamdaElement.Duration = Time;
	RamdaElement.StartValue = CurrentColor.A;
	RamdaElement.EndValue = 0.f;
	RamdaElement.TimerHandle = &TimerHandle;

	RamdaElement.PrevTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([this,RamdaElement, bAutoHideWhenFinished]() mutable
		{
			if (!this)
				return;
			RamdaElement.ElapsedTime += GetWorld()->GetTimeSeconds() - RamdaElement.PrevTime;
			float Alpha = FMath::Clamp(RamdaElement.ElapsedTime / RamdaElement.Duration, 0.f, 1.f);
			float CurValue = FMath::Lerp(RamdaElement.StartValue, RamdaElement.EndValue, Alpha);
			//UE_LOG(LogTemp, Warning, TEXT("Alpha %f"), Alpha * RamdaElement.Duration);
			RamdaElement.Color.A = CurValue;
			ImageScreen->SetColorAndOpacity(RamdaElement.Color);
			if (1.f <= Alpha)
			{
				RamdaElement.Color.A = 0.f;
				ImageScreen->SetColorAndOpacity(RamdaElement.Color);
				bPlay = false;
				if(bAutoHideWhenFinished)
					SetVisibility(ESlateVisibility::Collapsed);
				//UE_LOG(LogTemp, Warning, TEXT("End FadeIn"));
				GetWorld()->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);
			}
			RamdaElement.PrevTime = GetWorld()->GetTimeSeconds();
		}), 0.05f, true
	);
}
void UScreenEffectWidget:: FadeOut(float Time )
{
	if (bPlay)
		return;
	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	bPlay = true;
	FLinearColor CurrentColor = ImageScreen->ColorAndOpacity;
	CurrentColor.A = 0.f;
	ImageScreen->SetColorAndOpacity(CurrentColor);

	FRamdaScreenEffect RamdaElement;
	RamdaElement.Color = CurrentColor;
	RamdaElement.Duration = Time;
	RamdaElement.StartValue = CurrentColor.A;
	RamdaElement.EndValue = 1.f;
	RamdaElement.TimerHandle = &TimerHandle;
	RamdaElement.PrevTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([this, RamdaElement]() mutable
			{
				if (!this)
					return;
				RamdaElement.ElapsedTime += GetWorld()->GetTimeSeconds() - RamdaElement.PrevTime;
				float Alpha = FMath::Clamp(RamdaElement.ElapsedTime / RamdaElement.Duration, 0.f, 1.f);
				float CurValue = FMath::Lerp(RamdaElement.StartValue, RamdaElement.EndValue, Alpha);
				//UE_LOG(LogTemp, Warning, TEXT("Alpha %f"), Alpha* RamdaElement.Duration);
				RamdaElement.Color.A = CurValue;
				ImageScreen->SetColorAndOpacity(RamdaElement.Color);
				if (1.f <= Alpha)
				{
					RamdaElement.Color.A = 1.f;
					ImageScreen->SetColorAndOpacity(RamdaElement.Color);
					bPlay = false;
					GetWorld()->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);
					//UE_LOG(LogTemp, Warning, TEXT("TimerHandle"));
				}
				RamdaElement.PrevTime = GetWorld()->GetTimeSeconds();
			}), 0.05f, true
	);
}
void UScreenEffectWidget::FadeOutIn(float FadeOutTime , float DelayTime , float FadeInTime,bool bAutoHideWhenFinished)
{
	if (bPlay)
		return;
	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle2);

	FadeOut(FadeOutTime);
	//if (true)return;
	FRamdaScreenEffect RamdaElement;
	RamdaElement.TimerHandle = &TimerHandle2;
	RamdaElement.DelayTime = FadeOutTime+ DelayTime;
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([this, RamdaElement, FadeInTime, bAutoHideWhenFinished]() mutable
		{
			bPlay = false;
			//UE_LOG(LogTemp, Warning, TEXT("BlackTime"));
			FadeIn(FadeInTime, bAutoHideWhenFinished);
			GetWorld()->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);

		}), RamdaElement.DelayTime, false
	);
}