// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingRoomWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UOverlay;
class UImage;
class AWaitingRoomPlayerCardsRT;

UCLASS()
class ELVENRING_API UWaitingRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* NickNameFrameAppeare;

    UPROPERTY(meta = (BindWidget))
    UImage* ImageReady;
    UPROPERTY(meta = (BindWidget))
    UImage* Image_ShockWave1;
    UPROPERTY(meta = (BindWidget))
    UImage* Image_ShockWave2;
    UPROPERTY(meta = (BindWidget))
    UImage* Image_ShockWave3;

    UPROPERTY(meta = (BindWidget))
    UOverlay* OverlayNickNameFrame;

    UPROPERTY(meta = (BindWidget))
    UButton* OkNickNameButton;
   
    UPROPERTY(meta = (BindWidget))
    UButton* GameStartButton;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* NicknameTextBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TextPlayerCardNickName;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnStartButtonClicked();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnOkNickNameButtonClicked();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    AWaitingRoomPlayerCardsRT* WaitingRoomPlayerCardsRT;


    UFUNCTION(BlueprintCallable, Category = "UI")
    void GuestMode();
    UFUNCTION(BlueprintCallable, Category = "UI")
    void HostMode();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetPlayerCardRT(AWaitingRoomPlayerCardsRT* PlayerCardsRT);

    TArray<UImage*> ShockWaves;
protected:
    UFUNCTION(BlueprintCallable,Category = "Effect")
    void PlayShockWave(int32 idx);

    void OpenNamePopup();
  
    void CloseShockWave(FSlateBrush& Brush, UMaterialInstanceDynamic* DynMat, int idx);
    TArray<bool> bDegCntArr = {false,false,false };
    //bool bCheck;
    FTimerHandle TimerHandle1;
    TArray<FTimerHandle > ShockTimerHandles;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
};
