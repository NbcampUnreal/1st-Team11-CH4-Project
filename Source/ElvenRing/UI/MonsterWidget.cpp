// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

void UMonsterWidget::SetUiSize(FVector2D  Scale, FVector2D Pos)
{
	//CanvasPanel->SetRenderTransform(FVector2D(UiSize, UiSize));
	if (UWidget* CanvasAsWidget = Cast<UWidget>(CanvasPanelSlot))
	{
		CanvasAsWidget->SetRenderScale( Scale ); // ✅ 전체 UI 스케일 줄이기
		CanvasAsWidget->SetRenderTranslation( Pos );
	}
	if (true)
		return;

	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(HpProgressBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();
		
		ProgressBarCanvasSlot->SetSize(CurrentSize* Scale);
	}

	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(HpProgressYellowBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();

		ProgressBarCanvasSlot->SetSize(CurrentSize * Scale);
	}

	if (UCanvasPanelSlot* HpBarCanvasSlot = Cast<UCanvasPanelSlot>(HpFrameImg->Slot))
	{
		FVector2D CurrentSize = HpBarCanvasSlot->GetSize();

		HpBarCanvasSlot->SetSize(CurrentSize * Scale);
	}
}
void UMonsterWidget::DecreaseHp(float TargetHp, float HpMax)
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FMRamdaElement FElement;

	FElement.TargetProgressBarPer = TargetHp / HpMax;
	FElement.CurProgressBarPer = HpProgressYellowBar->GetPercent();
	FElement.MyProgressBar = HpProgressYellowBar;

	float Damage = FElement.CurProgressBarPer * HpMax - FElement.TargetProgressBarPer * HpMax;

	//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());

	FElement.DamageTextValue = FText::AsNumber(FMath::RoundToInt(Damage));

	HpProgressBar->SetPercent(FElement.TargetProgressBarPer);
	UpdateProgressBar(FElement);
}
void UMonsterWidget::RecoverHp(float TargetHp, float HpMax)
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FMRamdaElement FElement;
	FElement.Recover = true;
	FElement.Duration = 1.f;
	FElement.TargetProgressBarPer = TargetHp / HpMax;
	FElement.CurProgressBarPer = HpProgressYellowBar->GetPercent();
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyProgressYellowBar = HpProgressYellowBar;

	HpProgressBar->SetPercent(FElement.TargetProgressBarPer);
	UpdateProgressBar(FElement);
}

void UMonsterWidget::UpdateProgressBar(FMRamdaElement& FElement)
{
	if (!FElement.Recover)
		DamageText->SetText( FElement.DamageTextValue );

	GetWorld()->GetTimerManager().SetTimer
	(
		HpTimerDelayHandle,
		FTimerDelegate::CreateLambda([this, FElement]() mutable
			{
				FElement.PrevTime = GetWorld()->GetTimeSeconds();
				GetWorld()->GetTimerManager().SetTimer
				(
					HpTimerHandle,
					FTimerDelegate::CreateLambda([this, FElement]() mutable
					{
						FElement.ElapsedTime += GetWorld()->GetTimeSeconds() - FElement.PrevTime;
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
								DamageText->SetText( FText::GetEmpty());

							FElement.ClearPointer();
							GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
						}
						FElement.PrevTime = GetWorld()->GetTimeSeconds();
					}), 0.05f, true
				);
			}), 1.5f, false
	);
}
