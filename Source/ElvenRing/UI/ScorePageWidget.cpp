// Fill out your copyright notice in the Description page of Project Settings.


#include "ScorePageWidget.h"
#include "Components/TextBlock.h"
#include "Components/button.h"
#include "Kismet/GameplayStatics.h"

void UScorePageWidget::SetResultStat(int32 idx, EResultStat Stat, int Value)
{
	switch (Stat)
	{
	case EResultStat::DamageDealt:
		AllResultStats[idx].DamageDealt += Value;
		break;
	case EResultStat::DamageTaken:
		AllResultStats[idx].DamageTaken += Value;
		break;
	case EResultStat::RespawnCount:
		AllResultStats[idx].RespawnCount += Value;
		break;
	case EResultStat::RollCount:
		AllResultStats[idx].RollCount += Value;
		break;
	}
}

void UScorePageWidget::SetName(int32 idx, FString Name)
{
	PlayerNames[idx]->SetText(FText::FromString(Name));
}

void UScorePageWidget::OnClickOkBtn()
{
	UGameplayStatics::OpenLevel(this, FName("ElvenRuins_fix"));
}

void UScorePageWidget::NativeConstruct()
{
	AllResultStats.AddZeroed(2);
	OkButton->OnClicked.AddDynamic(this, &UScorePageWidget::OnClickOkBtn);
}

void UScorePageWidget::NativeDestruct()
{
}

void UScorePageWidget::CalculateResultStat()
{
	TextAttackValue1->SetText(FText::AsNumber( AllResultStats[0].DamageDealt));
	TextDamageValue1->SetText(FText::AsNumber(AllResultStats[0].DamageTaken));
	TextReSpwanCount1->SetText(FText::AsNumber(AllResultStats[0].RespawnCount));
	TextRollCount1->SetText(FText::AsNumber(AllResultStats[0].RollCount));

	TextAttackValue2->SetText(FText::AsNumber(AllResultStats[1].DamageDealt));
	TextDamageValue2->SetText(FText::AsNumber(AllResultStats[1].DamageTaken));
	TextReSpwanCount2->SetText(FText::AsNumber(AllResultStats[1].RespawnCount));
	TextRollCount2->SetText(FText::AsNumber(AllResultStats[1].RollCount));
}
