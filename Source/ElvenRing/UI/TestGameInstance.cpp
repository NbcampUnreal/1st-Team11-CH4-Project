// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/UI/TestGameInstance.h"
//#include "TestGameInstance.h"
#include "MessageWidget.h"

void UTestGameInstance::Init()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/WBP_MessageWidget"));
    if (WidgetClassFinder.Succeeded())
    {
        MessageWidgetClass = WidgetClassFinder.Class;
    }
}

void UTestGameInstance::ShowGlobalMessage(const FString& Message, float Duration)
{
    if (!MessageWidgetInstance && MessageWidgetClass)
    {
        MessageWidgetInstance = CreateWidget<UMessageWidget>(this, MessageWidgetClass);
        if (MessageWidgetInstance)
        {
            MessageWidgetInstance->AddToViewport();
        }
    }

    if (MessageWidgetInstance)
    {
        MessageWidgetInstance->ShowMessageText(Message);//FText::FromString(Message)
        MessageWidgetInstance->SetVisibility(ESlateVisibility::Visible);

        GetWorld()->GetTimerManager().ClearTimer(HideMessageTImerHandle);
        GetWorld()->GetTimerManager().SetTimer(HideMessageTImerHandle, [this]() 
            {
                if (MessageWidgetInstance)
                    MessageWidgetInstance->SetVisibility(ESlateVisibility::Hidden);

            }, Duration,false);
    }

}
