// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UMonsterWidget::SetSize(float size)
{
	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(HpProgressBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();
		
		ProgressBarCanvasSlot->SetSize(FVector2D(CurrentSize.X * size, CurrentSize.Y * size));
	}
	if (UCanvasPanelSlot* HpBarCanvasSlot = Cast<UCanvasPanelSlot>(HpFrameImg->Slot))
	{
		FVector2D CurrentSize = HpBarCanvasSlot->GetSize();

		HpBarCanvasSlot->SetSize(FVector2D(CurrentSize.X * size, CurrentSize.Y * size));
	}
}
void UMonsterWidget::UpdateHpBar(float Hp)
{
	HpProgressBar->SetPercent(1.f - (Hp / 100.f));
}
