// Fill out your copyright notice in the Description page of Project Settings
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommonTypes.h"
#include "UIManager.generated.h"

class UMainTitleWidget;
class UWaitingRoomWidget;
class UInGameWidget;
class UPlayerMainUi;
class UMessageWidget;
class UBattleMessageWidget;
class UInteractionMessageWidget;
class UBossWidget;
class UMonsterWidget;
class UMessageWidgetBase;
class UScreenEffectWidget;
class AWaitingRoomPlayerCardsRT;
class UScorePageWidget;
class AUnitBase;
//struct EResultStat;

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

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowTitleScreen(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowWaitingRoom(UWorld* World, AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetActiveCharactersUI(bool bActive);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowInGameUi(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowPlayerMainUi(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowInteractionMessage(UWorld* World);
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMessage(const FString& Message, EMessageType MsgType);

    FRenderTargetMaterialSet CreateRenderTargetMaterial(UObject* WorldContext, UMaterialInterface* BaseMaterial);
    
    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowScorePageWidget(UWorld* World);

    void ShowBossWidget(UWorld* World);

    void ClearAllWidgets();

    void DestroyMonsterHpWidget(AActor* Monster);

    void RegisterMessageWidgets();

    UPROPERTY(EditDefaultsOnly)
    UMaterialInterface* BaseRenderTargetMaterial;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UPlayerMainUi* GetPlayerMainUi() const { return PlayerMainUiWedget; };

    UFUNCTION(BlueprintCallable, Category = "UI")
    UWaitingRoomWidget* GetWaitingRoomUi() const { return WaitingRoomWidget; };

    UFUNCTION(BlueprintCallable, Category = "UI")
    UScreenEffectWidget* GetScreenEffectWidget();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void BindPlayerMainUi(UWorld* World, AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UBossWidget* GetBossWidgetUi() const { return BossWidget; };
    UFUNCTION(BlueprintCallable, Category = "UI")
    void BindBossWidgetUi(UWorld* World, AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CreateBindNormalMonsterWidgetUi(UWorld* World, AUnitBase* Unit);

    UFUNCTION(BlueprintCallable, Category = "UI")
    UScorePageWidget* GetScorePageWidget() const { return ScorePageWidget; };

    UFUNCTION(BlueprintCallable, Category = "UI")
    void UpdateGameResultStat(int32 idx, EResultStat Stat, int Value);

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
    UInteractionMessageWidget* InteractionMessageWidget;
    
    UPROPERTY()
    UPlayerMainUi* PlayerMainUiWedget;

    UPROPERTY()
    UMonsterWidget* MonsterWidget;

    UPROPERTY()
    TMap<AActor*, UMonsterWidget*> MonsterHpWidgets;

    UPROPERTY()
    UBossWidget* BossWidget;

    UPROPERTY()
    UScorePageWidget* ScorePageWidget;
    
    UPROPERTY()
    UScreenEffectWidget* ScreenEffectWidget;

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
    TSubclassOf<UInteractionMessageWidget> InteractionMessageWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPlayerMainUi> PlayerMainUiClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMonsterWidget> MonsterWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UBossWidget> BossWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UScorePageWidget> ScorePageWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UScreenEffectWidget> ScreenEffectWidgetClass;

private:
   // UWorld* CachedWorld = nullptr;
    TArray<UMessageWidgetBase*> MessageWidgets; 
private:

    //template <typename T>
    //TSubclassOf<T> LoadWidgetClass(const TCHAR* Path)
    //{
    //    ConstructorHelpers::FClassFinder<T> Finder(Path);
    //    return Finder.Succeeded() ? Finder.Class : nullptr;
    //};
};
