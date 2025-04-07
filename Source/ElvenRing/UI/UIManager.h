// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

class UMainTitleWidget;
class UWaitingRoomWidget;
class UInGameWidget;
class UPlayerMainUi;
class UMessageWidget;
class UBattleMessageWidget;
class UBossWidget;
class UMonsterWidget;
class UMessageWidgetBase;
class AUnitBase;

UENUM(BlueprintType)
enum class EMessageType : uint8
{
    SystemMessage = 0,
    BattleMessage = 1,
};

UCLASS(Blueprintable)
class ELVENRING_API UUIManager : public UObject
{
	GENERATED_BODY()

public:
    UUIManager();
    UFUNCTION(BlueprintCallable, Category = "UI")
    void InitUi(UWorld* WorldContext);

    void ShowTitleScreen();
    void ShowWaitingRoom();
    void ShowInGameUi();

    void ShowPlayerMainUi();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMessage(const FString& Message, EMessageType MsgType);

    void ShowBossWidget();

    void ClearAllWidgets();

    void RegisterMessageWidgets();

    UFUNCTION(BlueprintCallable, Category = "UI")
    UPlayerMainUi* GetPlayerMainUi() const { return PlayerMainUiWedget; };
    UFUNCTION(BlueprintCallable, Category = "UI")
    void BindPlayerMainUi(AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UBossWidget* GetBossWidgetUi() const { return BossWidget; };
    UFUNCTION(BlueprintCallable, Category = "UI")
    void BindBossWidgetUi(AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CreateBindNormalMonsterWidgetUi(AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UMessageWidgetBase* GetMessageWidgetSafe(EMessageType MsgType) const;

protected:
    UPROPERTY()
    UMainTitleWidget* TitleWidget;

    UPROPERTY()
    UWaitingRoomWidget* WaitingRoomWidget;

    UPROPERTY()
    UInGameWidget* InGameWidget;

    UPROPERTY()
    UMessageWidget* SystemMessageWidget;

    UPROPERTY()
    UBattleMessageWidget* BattleMessageWidget;

    UPROPERTY()
    UPlayerMainUi* PlayerMainUiWedget;

    UPROPERTY()
    UMonsterWidget* MonsterWidget;

    UPROPERTY()
    UBossWidget* BossWidget;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMainTitleWidget> TitleWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UWaitingRoomWidget> WaitingRoomWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UInGameWidget> InGameWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMessageWidget> SystemMessageWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBattleMessageWidget> BattleMessageWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPlayerMainUi> PlayerMainUiClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMonsterWidget> MonsterWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBossWidget> BossWidgetClass;
  
private:
    UWorld* CachedWorld = nullptr;
    TArray<UMessageWidgetBase*> MessageWidgets; 
private:

    //template <typename T>
    //TSubclassOf<T> LoadWidgetClass(const TCHAR* Path)
    //{
    //    ConstructorHelpers::FClassFinder<T> Finder(Path);
    //    return Finder.Succeeded() ? Finder.Class : nullptr;
    //};
};
