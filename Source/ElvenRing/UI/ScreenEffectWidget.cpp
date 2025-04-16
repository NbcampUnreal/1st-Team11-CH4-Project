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

	TWeakObjectPtr<UScreenEffectWidget> SafeThis = this;

	RamdaElement.PrevTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([SafeThis,RamdaElement, bAutoHideWhenFinished]() mutable
		{
			if (!SafeThis.IsValid())
				return;
			UWorld* World = SafeThis->GetWorld();
			if (!World)return;

			RamdaElement.ElapsedTime += World->GetTimeSeconds() - RamdaElement.PrevTime;
			float Alpha = FMath::Clamp(RamdaElement.ElapsedTime / RamdaElement.Duration, 0.f, 1.f);
			float CurValue = FMath::Lerp(RamdaElement.StartValue, RamdaElement.EndValue, Alpha);
			//UE_LOG(LogTemp, Warning, TEXT("Alpha %f"), Alpha * RamdaElement.Duration);
			RamdaElement.Color.A = CurValue;
			SafeThis->ImageScreen->SetColorAndOpacity(RamdaElement.Color);
			if (1.f <= Alpha)
			{
				RamdaElement.Color.A = 0.f;
				SafeThis->ImageScreen->SetColorAndOpacity(RamdaElement.Color);
				SafeThis->bPlay = false;
				if(bAutoHideWhenFinished)
					SafeThis->SetVisibility(ESlateVisibility::Collapsed);
				//UE_LOG(LogTemp, Warning, TEXT("End FadeIn"));
				World->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);
			}
			else
				RamdaElement.PrevTime = World->GetTimeSeconds();
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

	TWeakObjectPtr<UScreenEffectWidget> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, RamdaElement]() mutable
			{
				if (!SafeThis.IsValid())
					return;
				UWorld* World = SafeThis->GetWorld();
				if (!World)return;

				RamdaElement.ElapsedTime += World->GetTimeSeconds() - RamdaElement.PrevTime;
				float Alpha = FMath::Clamp(RamdaElement.ElapsedTime / RamdaElement.Duration, 0.f, 1.f);
				float CurValue = FMath::Lerp(RamdaElement.StartValue, RamdaElement.EndValue, Alpha);
				//UE_LOG(LogTemp, Warning, TEXT("Alpha %f"), Alpha* RamdaElement.Duration);
				RamdaElement.Color.A = CurValue;
				SafeThis->ImageScreen->SetColorAndOpacity(RamdaElement.Color);
				if (1.f <= Alpha)
				{
					RamdaElement.Color.A = 1.f;
					SafeThis->ImageScreen->SetColorAndOpacity(RamdaElement.Color);
					SafeThis->bPlay = false;
					World->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);
					//UE_LOG(LogTemp, Warning, TEXT("TimerHandle"));
				}
				else
					RamdaElement.PrevTime = World->GetTimeSeconds();
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

	TWeakObjectPtr<UScreenEffectWidget> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		*RamdaElement.TimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, RamdaElement, FadeInTime, bAutoHideWhenFinished]() mutable
		{
			if (!SafeThis.IsValid())
				return;
			UWorld* World = SafeThis->GetWorld();
			if (!World)return;

			SafeThis->bPlay = false;
			//UE_LOG(LogTemp, Warning, TEXT("BlackTime"));
			SafeThis->bPlay = false;
			SafeThis->FadeIn(FadeInTime, bAutoHideWhenFinished);
			World->GetTimerManager().ClearTimer(*RamdaElement.TimerHandle);

		}), RamdaElement.DelayTime, false
	);
}