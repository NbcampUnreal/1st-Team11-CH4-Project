// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHpBaseWidget.h"
#include "Components/CanvasPanel.h"



UMonsterHpBaseWidget::UMonsterHpBaseWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void UMonsterHpBaseWidget::SetUiSize(FVector2D Scale, FVector2D Pos)
{
	if (UWidget* CanvasAsWidget = Cast<UWidget>(CanvasPanelSlot))
	{
		CanvasAsWidget->SetRenderScale(Scale);
		CanvasAsWidget->SetRenderTranslation(Pos);
	}
}

void UMonsterHpBaseWidget::DecreaseHp(float CurHp, float HpMax)
{

}

void UMonsterHpBaseWidget::RecoverHp(float TargetHp, float HpMax)
{
}


void UMonsterHpBaseWidget::UpdateProgressBar(FMRamdaElement& FElement)
{
}

