// Fill out your copyright notice in the Description page of Project Settings.



#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MessageWidget.h"
#include "MainTitleWidget.h"
#include "WaitingRoomWidget.h"
#include "InGameWidget.h"
#include "PlayerMainUi.h"
#include "BattleMessageWidget.h"
#include "BossWidget.h"
#include "MonsterWidget.h"
#include "MessageWidgetBase.h"

UUIManager::UUIManager()
{
   // if (true) return;
   
    //블루프린트에서 클래스 연결을 시켜 놓긴 했음.  
    static ConstructorHelpers::FClassFinder<UMainTitleWidget> WBP_TitleWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_MainTitle"));
    if (WBP_TitleWidget.Succeeded())
        TitleWidgetClass = WBP_TitleWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("TitleWidgetClass: %s"), *GetNameSafe(TitleWidgetClass));
    static ConstructorHelpers::FClassFinder<UWaitingRoomWidget> WBP_WaitingRoomWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_WaitingRoomWidget"));
    if (WBP_WaitingRoomWidget.Succeeded())
        WaitingRoomWidgetClass = WBP_WaitingRoomWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("WaitingRoomWidgetClass: %s"), *GetNameSafe(WaitingRoomWidgetClass));

    static ConstructorHelpers::FClassFinder<UInGameWidget> WBP_InGameWidgetClass(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_InGameWidget"));
    if (WBP_InGameWidgetClass.Succeeded())
        InGameWidgetClass = WBP_InGameWidgetClass.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("InGameWidgetClass: %s"), *GetNameSafe(InGameWidgetClass));

    static ConstructorHelpers::FClassFinder<UMessageWidget> WBP_SystemMessageWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_Message"));
    if (WBP_SystemMessageWidget.Succeeded())
        SystemMessageWidgetClass = WBP_SystemMessageWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("SystemMessageWidgetClass: %s"), *GetNameSafe(SystemMessageWidgetClass));

    static ConstructorHelpers::FClassFinder<UBattleMessageWidget> WBP_BattleMessageWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_BattleMessageWidget"));
    if (WBP_BattleMessageWidget.Succeeded())
        BattleMessageWidgetClass = WBP_BattleMessageWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("BattleMessageWidgetClass: %s"), *GetNameSafe(BattleMessageWidgetClass));

    static ConstructorHelpers::FClassFinder<UPlayerMainUi> WBP_PlayerMainUiWedget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_PlayerMainUi"));
    if (WBP_PlayerMainUiWedget.Succeeded())
        PlayerMainUiClass = WBP_PlayerMainUiWedget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("PlayerMainUiClass: %s"), *GetNameSafe(PlayerMainUiClass));

    static ConstructorHelpers::FClassFinder<UBossWidget> WBP_BossWidgetClass(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_Boss"));
    if (WBP_BossWidgetClass.Succeeded())
        BossWidgetClass = WBP_BossWidgetClass.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("BossWidgetClass: %s"), *GetNameSafe(BossWidgetClass));
}

void UUIManager::InitUi(UWorld* WorldContext)
{
    CachedWorld = WorldContext;
    if (TitleWidgetClass) //  UE_LOG(LogTemp, Warning, TEXT("UIManager instance class: %s"), *GetNameSafe(UIManager->GetClass()));
        TitleWidget = CreateWidget<UMainTitleWidget>(CachedWorld, TitleWidgetClass);

    if (WaitingRoomWidgetClass)
        WaitingRoomWidget = CreateWidget<UWaitingRoomWidget>(CachedWorld, WaitingRoomWidgetClass);

    if (InGameWidgetClass)
        InGameWidget = CreateWidget<UInGameWidget>(CachedWorld, InGameWidgetClass);

    if (SystemMessageWidgetClass)
        SystemMessageWidget = CreateWidget<UMessageWidget>(CachedWorld, SystemMessageWidgetClass);

    if (BattleMessageWidgetClass)
        BattleMessageWidget = CreateWidget<UBattleMessageWidget>(CachedWorld, BattleMessageWidgetClass);

    if (PlayerMainUiClass)
        PlayerMainUiWedget = CreateWidget<UPlayerMainUi>(CachedWorld, PlayerMainUiClass);

    if (BossWidgetClass)
        BossWidget = CreateWidget<UBossWidget>(CachedWorld, BossWidgetClass);

    RegisterMessageWidgets();
}

void UUIManager::ShowTitleScreen()
{
    if (!TitleWidget && TitleWidgetClass && CachedWorld)
        TitleWidget = CreateWidget<UMainTitleWidget>(CachedWorld, TitleWidgetClass);

    if (TitleWidget)
        TitleWidget->AddToViewport();
}

void UUIManager::ShowWaitingRoom()
{
    if (!WaitingRoomWidget && WaitingRoomWidgetClass && CachedWorld)
        WaitingRoomWidget = CreateWidget<UWaitingRoomWidget>(CachedWorld, WaitingRoomWidgetClass);

    if (WaitingRoomWidget)
        WaitingRoomWidget->AddToViewport();
}

void UUIManager::ShowInGameUi()
{
    if (!InGameWidget && InGameWidgetClass && CachedWorld)
        InGameWidget = CreateWidget<UInGameWidget>(CachedWorld, InGameWidgetClass);

    if (InGameWidget)
        InGameWidget->AddToViewport();
}

void UUIManager::ShowMessage(const FString& Message, EMessageType MsgType)
{
   UMessageWidgetBase* Widget = GetMessageWidgetSafe(MsgType);
   if (Widget)
   {
       Widget->AddToViewport();
       Widget->ShowMessageText(Message);
   }
}

void UUIManager::ShowBossWidget()
{
    if (!BossWidget && BossWidgetClass && CachedWorld)
        BossWidget = CreateWidget<UBossWidget>(CachedWorld, BossWidgetClass);

    if (BossWidget)
        BossWidget->AddToViewport();
}

void UUIManager::ClearAllWidgets()
{
    TArray<UUserWidget*> Widgets = { TitleWidget, WaitingRoomWidget, InGameWidget, SystemMessageWidget, BattleMessageWidget, BossWidget };

    for (UUserWidget* Widget : Widgets)
    {
        if (Widget && Widget->IsInViewport())
        {
            Widget->RemoveFromParent();
        }
    }

    TitleWidget = nullptr;
    WaitingRoomWidget = nullptr;
    InGameWidget = nullptr;
    SystemMessageWidget = nullptr;
    BattleMessageWidget = nullptr;
    PlayerMainUiWedget = nullptr;
    BossWidget = nullptr;
}

void UUIManager::RegisterMessageWidgets()
{
    MessageWidgets.Add( SystemMessageWidget);
    MessageWidgets.Add( BattleMessageWidget);
}

UMessageWidgetBase* UUIManager::GetMessageWidgetSafe(EMessageType MsgType) const
{
    uint8 Index = static_cast<uint8>(MsgType);
    if (MessageWidgets.IsValidIndex(Index))
    {
        return MessageWidgets[Index];
    }
    return nullptr;
}



