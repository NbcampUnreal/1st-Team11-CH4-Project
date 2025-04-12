// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "ElvenRing/Character/UnitBase.h"
#include "ElvenRing/NormalAI/NormalMonster.h"
#include "Components/WidgetComponent.h" //ksw

void UMonsterWidget::SetUiSize(FVector2D  Scale, FVector2D Pos)
{
	//CanvasPanel->SetRenderTransform(FVector2D(UiSize, UiSize));
	if (UWidget* CanvasAsWidget = Cast<UWidget>(CanvasPanelSlot))
	{
		CanvasAsWidget->SetRenderScale( Scale );
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
	if (KINDA_SMALL_NUMBER >= HpProgressBar->GetPercent())
		return;

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
void UMonsterWidget::RecoverHp(float TargetHp, float HpMax)
{
	if (1.f <= HpProgressBar->GetPercent())
		return;

	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);
	
	FMRamdaElement FElement;
	FElement.Recover = true;
	FElement.Duration = 1.f;
	FElement.TargetProgressBarPer = TargetHp / HpMax;
	FElement.CurProgressBarPer = HpProgressBar->GetPercent();
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyProgressYellowBar = HpProgressYellowBar;
	UpdateProgressBar(FElement);
	//HpProgressBar->SetPercent(FElement.TargetProgressBarPer);
	//HpProgressYellowBar->SetPercent(FElement.TargetProgressBarPer);
	UpdateProgressBar(FElement);
}
void UMonsterWidget::UpdateHp(float TargetHp, float HpMax, int32 State)
{
	if (State == 0)
		DecreaseHp(TargetHp, HpMax);
	else
		RecoverHp(TargetHp, HpMax);
}
void UMonsterWidget::BindToMonster(AUnitBase* Monster)
{
	if (Monster)
	{
		Cast< ANormalMonster >(Monster)->SetWidget(this);
		Monster->OnHpChanged.AddDynamic(this, &UMonsterWidget::UpdateHp);
		TempMonster = Monster;
	}
}

void UMonsterWidget::SetActiveWidget(bool bShow)
{
	if (bShow)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}

void UMonsterWidget::NativeConstruct()
{
	SetUiSize(UIScale,UIPos);
}
void UMonsterWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UMonsterWidget::UpdateProgressBar(FMRamdaElement& FElement)
{
	if (FElement.DelayTime <= 0.f) //람다 DelayTime이 0이면 진입을 못한다.
		FElement.DelayTime = 0.01f;
	if (!FElement.Recover)
		DamageText->SetText( FElement.DamageTextValue );

	GetWorld()->GetTimerManager().SetTimer
	(
		HpTimerDelayHandle,
		FTimerDelegate::CreateLambda([this, FElement]() mutable
			{
				if (!this)
				{
					//UE_LOG(LogTemp, Warning, TEXT("this ERROR!!!!!!!!!!!!! "));
					return;
				}

				FElement.PrevTime = GetWorld()->GetTimeSeconds();
				GetWorld()->GetTimerManager().SetTimer
				(
					HpTimerHandle,
					FTimerDelegate::CreateLambda([this, FElement]() mutable
					{
							if (!this)
							{
								//UE_LOG(LogTemp, Warning, TEXT("this ERROR!!!!!!!!!!!!! "));
								return;
							}
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
							{
								DamageText->SetText(FText::GetEmpty());
								if (KINDA_SMALL_NUMBER >= FElement.MyProgressBar->GetPercent())
								{
									TempMonster->OnHpChanged.RemoveDynamic(this, &UMonsterWidget::UpdateHp);
									TempMonster = nullptr;
									//UE_LOG(LogTemp, Warning, TEXT(" RemoveDynamic!!!!!! Monster"));
								}
							}
							FElement.ClearPointer();
							GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
						}
						FElement.PrevTime = GetWorld()->GetTimeSeconds();
					}), 0.05f, true
				);
			}), FElement.DelayTime, false
	);
}
