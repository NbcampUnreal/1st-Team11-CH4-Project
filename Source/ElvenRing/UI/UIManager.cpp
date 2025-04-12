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
#include "ScreenEffectWidget.h"
#include "ElvenRing/Character/UnitBase.h"
#include "Components/EditableTextBox.h"

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

    static ConstructorHelpers::FClassFinder<UBossWidget> WBP_BossWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_Boss"));
    if (WBP_BossWidget.Succeeded())
        BossWidgetClass = WBP_BossWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("BossWidgetClass: %s"), *GetNameSafe(BossWidgetClass));

    static ConstructorHelpers::FClassFinder<UScreenEffectWidget> WBP_ScreenEffectWidget(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_ScreenEffectWidget"));
    if (WBP_ScreenEffectWidget.Succeeded())
        ScreenEffectWidgetClass = WBP_ScreenEffectWidget.Class;
    else
        UE_LOG(LogTemp, Warning, TEXT("ScreenEffectWidgetClass: %s"), *GetNameSafe(ScreenEffectWidgetClass));
}

void UUIManager::InitUi(UWorld* World)
{

    if (TitleWidgetClass) //  UE_LOG(LogTemp, Warning, TEXT("UIManager instance class: %s"), *GetNameSafe(UIManager->GetClass()));
        TitleWidget = CreateWidget<UMainTitleWidget>(World, TitleWidgetClass);

    if (WaitingRoomWidgetClass)
        WaitingRoomWidget = CreateWidget<UWaitingRoomWidget>(World, WaitingRoomWidgetClass);

    if (InGameWidgetClass)
        InGameWidget = CreateWidget<UInGameWidget>(World, InGameWidgetClass);

    if (SystemMessageWidgetClass)
        SystemMessageWidget = CreateWidget<UMessageWidget>(World, SystemMessageWidgetClass);

    if (BattleMessageWidgetClass)
        BattleMessageWidget = CreateWidget<UBattleMessageWidget>(World, BattleMessageWidgetClass);

    if (PlayerMainUiClass)
        PlayerMainUiWedget = CreateWidget<UPlayerMainUi>(World, PlayerMainUiClass);

    if (BossWidgetClass)
        BossWidget = CreateWidget<UBossWidget>(World, BossWidgetClass);

    if (ScreenEffectWidgetClass)
        ScreenEffectWidget = CreateWidget<UScreenEffectWidget>(World, ScreenEffectWidgetClass);
    
    RegisterMessageWidgets();
}

void UUIManager::ShowTitleScreen(UWorld* World)
{
    if (!TitleWidget && TitleWidgetClass && World)
        TitleWidget = CreateWidget<UMainTitleWidget>(World, TitleWidgetClass);

    if (TitleWidget)
    {
        TitleWidget->AddToViewport();

        if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
           // InputMode.SetWidgetToFocus(TitleWidget->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PC->SetInputMode(InputMode);
        }
    }
}

void UUIManager::ShowWaitingRoom(UWorld* World)
{
    if (!WaitingRoomWidget && WaitingRoomWidgetClass && World)
        WaitingRoomWidget = CreateWidget<UWaitingRoomWidget>(World, WaitingRoomWidgetClass);

    if (WaitingRoomWidget)
    {
        WaitingRoomWidget->AddToViewport();

        if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
        {
            PC->bShowMouseCursor = true;

            FInputModeUIOnly InputMode;
            
            InputMode.SetWidgetToFocus(WaitingRoomWidget->NicknameTextBox->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

            PC->SetInputMode(InputMode);
        }
    }
}
void UUIManager::SetActiveCharactersUI(bool bActive)
{
    if (PlayerMainUiWedget)
        PlayerMainUiWedget->SetActiveWidget(bActive);

    if (BossWidget)
        BossWidget->SetActiveWidget(bActive);
   
    for (const TPair<AActor*, UMonsterWidget*>& Pair : MonsterHpWidgets)
        if (UMonsterWidget* Widget = Pair.Value)
            Widget->SetActiveWidget(bActive);

    /* for (UMessageWidgetBase* Widget : MessageWidgets)
    {
        if (Widget)
            Widget->SetActiveWidget(bActive);
    }*/
}
void UUIManager::ShowInGameUi(UWorld* World)
{
    if (!InGameWidget && InGameWidgetClass && World)
        InGameWidget = CreateWidget<UInGameWidget>(World, InGameWidgetClass);

    if (InGameWidget&& !InGameWidget->IsInViewport())
        InGameWidget->AddToViewport();
}
void UUIManager::ShowPlayerMainUi(UWorld* World)
{
    if (!PlayerMainUiWedget && PlayerMainUiClass && World)
        PlayerMainUiWedget = CreateWidget<UPlayerMainUi>(World, PlayerMainUiClass);

    if (PlayerMainUiWedget && !PlayerMainUiWedget->IsInViewport())
        PlayerMainUiWedget->AddToViewport();

    if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
    {
        PC->bShowMouseCursor = false;
        // 게임 전용 입력 모드로 전환
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }

}
void UUIManager::ShowMessage(const FString& Message, EMessageType MsgType)
{
   UMessageWidgetBase* Widget = GetMessageWidgetSafe(MsgType);
   if (Widget)
   {
       if(!Widget->IsInViewport() )
         Widget->AddToViewport();
       Widget->ShowMessageText(Message);
   }
}

void UUIManager::ShowBossWidget(UWorld* World)
{
    if (!BossWidget && BossWidgetClass && World)
        BossWidget = CreateWidget<UBossWidget>(World, BossWidgetClass);

    if ( BossWidget && !BossWidget->IsInViewport())
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
    MonsterWidget = nullptr;
    BossWidget = nullptr;
    ScreenEffectWidget = nullptr;

    MonsterHpWidgets.Empty(); 
}
void UUIManager::DestroyMonsterHpWidget(AActor* Monster)
{
    if (MonsterHpWidgets.Contains(Monster))
    {
        UMonsterWidget* Widget = MonsterHpWidgets[Monster];
        if (Widget)
            Widget->RemoveFromParent();

        MonsterHpWidgets.Remove(Monster);
    }
}
void UUIManager::RegisterMessageWidgets()
{
    MessageWidgets.Add( SystemMessageWidget);
    MessageWidgets.Add( BattleMessageWidget);
}

UScreenEffectWidget* UUIManager::GetScreenEffectWidget()
{
    if (ScreenEffectWidget && !ScreenEffectWidget->IsInViewport())
    {
        ScreenEffectWidget->AddToViewport(100);
    }
    return ScreenEffectWidget;
}

void UUIManager::BindPlayerMainUi(UWorld* World, AUnitBase* Unit)
{
    ShowPlayerMainUi(World);
    if(GetPlayerMainUi())
         GetPlayerMainUi()->BindToPlayer(Unit);
    else
        UE_LOG(LogTemp, Warning, TEXT("Error!! BindPlayerMainUi"));
}

void UUIManager::BindBossWidgetUi(UWorld* World, AUnitBase* Unit)
{
    ShowBossWidget(World);
    if(BossWidget)
        BossWidget->BindToBoss(Unit);
    else
        UE_LOG(LogTemp, Warning, TEXT("Error!! BindBossWidgetUi"));
}

void UUIManager::CreateBindNormalMonsterWidgetUi(UWorld* World,AUnitBase* Unit)
{
    UMonsterWidget* MonsterUiWidget = nullptr;
    if (MonsterWidgetClass && World)
    {
        MonsterUiWidget = CreateWidget<UMonsterWidget>(World, MonsterWidgetClass);
        if (MonsterUiWidget)
        {
            MonsterUiWidget->BindToMonster(Unit);
            MonsterHpWidgets.Add(Unit,MonsterUiWidget);
        }
        else
            UE_LOG(LogTemp, Warning, TEXT("Error!! CreateBindNormalMonsterWidgetUi"));
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("Error!! CreateBindNormalMonsterWidgetUi MonsterWidgetClass World"));
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


