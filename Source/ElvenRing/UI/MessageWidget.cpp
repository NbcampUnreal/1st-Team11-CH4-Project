// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/MessageWidget.h"
#include "MessageWidget.h"
#include "Components/TextBlock.h"

void UMessageWidget::ShowMessageText_Implementation(const FString& Message)
{
    if (GetVisibility() == ESlateVisibility::Collapsed)
    SetVisibility(ESlateVisibility::Visible);

    TextMessage->SetText(FText::FromString(Message));
    PlayFadeAnimation();
}

//void UMessageWidget::ShowMessageText(const FString& Message)
//{
//    if (GetVisibility() == ESlateVisibility::Collapsed)
//        SetVisibility(ESlateVisibility::Visible);
//
//    TextMessage->SetText(FText::FromString(Message));
//    PlayFadeAnimation();
//}

void UMessageWidget::NativeConstruct()
{
    Super::NativeConstruct();
    bCollapsed = true;
}

void UMessageWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
