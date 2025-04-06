// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleMessageWidget.h"
#include "Components/TextBlock.h"


void UBattleMessageWidget::ShowMessageText_Implementation(const FString& Message)
{
    if (GetVisibility() == ESlateVisibility::Collapsed)
        SetVisibility(ESlateVisibility::Visible);

    TextMessage->SetText(FText::FromString(Message));
    PlayFadeAnimation();
}
//void UBattleMessageWidget::ShowMessageText(const FString& Message)
//{
//    if (GetVisibility() == ESlateVisibility::Collapsed)
//        SetVisibility(ESlateVisibility::Visible);
//
//    TextMessage->SetText(FText::FromString(Message));
//    PlayFadeAnimation();
//}

void UBattleMessageWidget::NativeConstruct()
{
    Super::NativeConstruct();
    bCollapsed = true;
}

void UBattleMessageWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
