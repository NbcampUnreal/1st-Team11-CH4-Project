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
#include <Kismet/GameplayStatics.h>

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

void UMonsterWidget::RealTimeInGameWidgetScale()
{
	
	FMRamdaElement FElement;
	TWeakObjectPtr<UMonsterWidget> SafeThis = this;
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyProgressYellowBar = HpProgressYellowBar;
	FElement.HpTimerHandle = &HpScaleTimerHandle;
	UWidget* CanvasAsWidget = Cast<UWidget>(CanvasPanelSlot);
	if (!CanvasAsWidget)return;

	FVector2D Scale = UIScale;
	FVector2D Pos = UIPos;
	GetWorld()->GetTimerManager().SetTimer
	(
		*FElement.HpTimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, Scale, Pos, CanvasAsWidget, FElement]() mutable
		{
			if (!SafeThis.IsValid())
				return;
			UWorld* World = SafeThis->GetWorld();
			if (!World)return;

			APlayerController* PC = UGameplayStatics::GetPlayerController(SafeThis.Get(), 0);
			if (!PC) return;

			APawn* PlayerPawn = PC->GetPawn();
			if (!PlayerPawn) return;

			float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), SafeThis->TempMonster->GetActorLocation());

			float ScalePer = FMath::Clamp( SafeThis->HpBarSizeCalculMeasureDistLimit / Distance, SafeThis->HpBarSizeScreenMin, Scale.X);

			//UE_LOG(LogTemp, Warning, TEXT("Distance : %f / ScalePer :  %f "), Distance, ScalePer);

			if (KINDA_SMALL_NUMBER >= FElement.MyProgressBar->GetPercent())
			{
				if (World)
				{
					FElement.ClearPointer();
					World->GetTimerManager().ClearTimer(*FElement.HpTimerHandle);
					return;
				}
			}
			else
			{
				if (CanvasAsWidget)
				{
					CanvasAsWidget->SetRenderScale(FVector2D(ScalePer, ScalePer));
					float FixHeightPosPercent = ScalePer / Scale.X;
					FVector2D FixHeightPos = Pos * FixHeightPosPercent;
					CanvasAsWidget->SetRenderTranslation(FixHeightPos);

					float OpacityPercent = FMath::Clamp(FixHeightPosPercent*1.75f, 0.f, 1.f);
					CanvasAsWidget->SetRenderOpacity(OpacityPercent);
				}
				else
				{
					if (World)
					{
						FElement.ClearPointer();
						World->GetTimerManager().ClearTimer(*FElement.HpTimerHandle);
						return;
					}
				}
			}
		}), 0.05f, true
	);
}

void UMonsterWidget::NativeConstruct()
{
	SetUiSize(UIScale,UIPos);
	RealTimeInGameWidgetScale();
}
void UMonsterWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if(TempMonster)
		TempMonster->OnHpChanged.RemoveDynamic(this, &UMonsterWidget::UpdateHp);
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);
		GetWorld()->GetTimerManager().ClearTimer(HpScaleTimerHandle);
	}
}

//void UMonsterWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//
//}

void UMonsterWidget::UpdateProgressBar(FMRamdaElement& FElement)
{
	if (FElement.DelayTime <= 0.f) //람다 DelayTime이 0이면 진입을 못한다.
		FElement.DelayTime = 0.01f;
	if (!FElement.Recover)
		DamageText->SetText( FElement.DamageTextValue );

	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);

	FElement.HpTimerDelayHandle = &HpTimerDelayHandle;
	FElement.HpTimerHandle = &HpTimerHandle;

	TWeakObjectPtr<UMonsterWidget> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		*FElement.HpTimerDelayHandle,
		FTimerDelegate::CreateLambda([SafeThis,FElement]() mutable
			{
				if (!SafeThis.IsValid())return;
				UWorld* World = SafeThis->GetWorld();
				if (!World)return;

				FElement.PrevTime = World->GetTimeSeconds();
				World->GetTimerManager().SetTimer
				(
					*FElement.HpTimerHandle,
					FTimerDelegate::CreateLambda([SafeThis, FElement]() mutable
					{
						if (!SafeThis.IsValid())
							return;
						UWorld* World = SafeThis->GetWorld();
						if (!World)return;

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
							{
								SafeThis->DamageText->SetText(FText::GetEmpty());
								if (KINDA_SMALL_NUMBER >= FElement.MyProgressBar->GetPercent())
								{
									SafeThis->TempMonster->OnHpChanged.RemoveDynamic(SafeThis.Get(), &UMonsterWidget::UpdateHp);
									SafeThis->TempMonster = nullptr;
									//UE_LOG(LogTemp, Warning, TEXT(" RemoveDynamic!!!!!! Monster"));
								}
							}
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
