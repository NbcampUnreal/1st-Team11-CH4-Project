// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingRoomWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class  UOverlay;
UCLASS()
class ELVENRING_API UWaitingRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
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

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
};
