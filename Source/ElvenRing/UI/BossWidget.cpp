// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWidget.h"
#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "ElvenRing/Character/UnitBase.h"

void UBossWidget::SetUiSize(FVector2D Scale, FVector2D Pos)
{
	if (UWidget* CanvasAsWidget = Cast<UWidget>(CanvasPanelSlot))
	{
		CanvasAsWidget->SetRenderScale(Scale);
		CanvasAsWidget->SetRenderTranslation(Pos);
	}
}
void UBossWidget::UpdateHp(float TargetHp, float HpMax, int32 State)
{
	if (State == 0)
		DecreaseHp(TargetHp, HpMax);
	else
		RecoverHp(TargetHp, HpMax);
}
void UBossWidget::DecreaseHp(float TargetHp, float HpMax)
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FMRamdaElement FElement;

	FElement.TargetProgressBarPer = TargetHp / HpMax;
	FElement.CurProgressBarPer = HpProgressYellowBar->GetPercent();
	FElement.MyProgressBar = HpProgressYellowBar;
	FElement.Duration = 1.f;
	FElement.DelayTime = 1.f;

	float Damage = FElement.CurProgressBarPer * HpMax - FElement.TargetProgressBarPer * HpMax;

	//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());

	FElement.DamageTextValue = FText::AsNumber(FMath::RoundToInt(Damage));

	HpProgressBar->SetPercent(FElement.TargetProgressBarPer);
	UpdateProgressBar(FElement);
}

void UBossWidget::RecoverHp(float TargetHp, float HpMax)
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FMRamdaElement FElement;
	FElement.Recover = true;
	FElement.Duration = 1.f;
	FElement.DelayTime = 0.1f;
	FElement.TargetProgressBarPer = TargetHp / HpMax;
	FElement.CurProgressBarPer = HpProgressYellowBar->GetPercent();
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyProgressYellowBar = HpProgressYellowBar;

	HpProgressYellowBar->SetPercent(FElement.TargetProgressBarPer);
	UpdateProgressBar(FElement);
}

void UBossWidget::UpdateProgressBar(FMRamdaElement& FElement)
{
	if (FElement.DelayTime <= 0.f) //람다 DelayTime이 0이면 진입을 못한다.
		FElement.DelayTime = 0.01f;

	if (!FElement.Recover)
		DamageText->SetText(FElement.DamageTextValue);

	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);

	FElement.HpTimerDelayHandle = &HpTimerDelayHandle;
	FElement.HpTimerHandle = &HpTimerHandle;

	TWeakObjectPtr<UBossWidget> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		*FElement.HpTimerDelayHandle,
		FTimerDelegate::CreateLambda([SafeThis, FElement]() mutable
			{
				if (!SafeThis.IsValid())
					return;
				UWorld* World = SafeThis->GetWorld();
				if (!World)
					return;

				FElement.PrevTime = World->GetTimeSeconds();
				World->GetTimerManager().SetTimer
				(
					*FElement.HpTimerHandle,
					FTimerDelegate::CreateLambda([SafeThis, FElement]() mutable
						{
							if (!SafeThis.IsValid())
								return;
							UWorld* World = SafeThis->GetWorld();
							if (!World)
								return;

							FElement.ElapsedTime += World->GetTimeSeconds() - FElement.PrevTime;
							float Alpha = FMath::Clamp(FElement.ElapsedTime / FElement.Duration, 0.f, 1.f);
							float CurValue = FMath::Lerp(FElement.CurProgressBarPer, FElement.TargetProgressBarPer, Alpha);
							//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());
							FElement.MyProgressBar->SetPercent(CurValue);

							if (1.f <= Alpha)
							{
								FElement.MyProgressBar->SetPercent(FElement.TargetProgressBarPer);
								if (FElement.Recover)
									FElement.MyProgressYellowBar->SetPercent(FElement.TargetProgressBarPer);
								else
									SafeThis->DamageText->SetText(FText::GetEmpty());

								FElement.ClearPointer();
								World->GetTimerManager().ClearTimer(*FElement.HpTimerHandle);
							}
							else
								FElement.PrevTime = World->GetTimeSeconds();
						}), 0.05f, true
				);
			}), FElement.DelayTime, false
	);
}
void UBossWidget::BindToBoss(AUnitBase* Boss)
{
	if (Boss)
	{
		Boss->OnHpChanged.AddDynamic(this, &UBossWidget::UpdateHp);
		//NameText->SetText(FText::FromString(Name));
	}
}
void UBossWidget::SetName(FString Name)
{
	NameText->SetText(FText::FromString(Name));
}
void UBossWidget::SetActiveWidget(bool bShow)
{
	if (bShow)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}
void UBossWidget::NativeConstruct()
{
	SetUiSize(UIScale, UIPos);
}
void UBossWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}
}