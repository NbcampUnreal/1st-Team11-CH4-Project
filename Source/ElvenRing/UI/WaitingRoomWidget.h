// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaitingRoomWidget.generated.h"

class UEditableTextBox;
class UButton;
UCLASS()
class ELVENRING_API UWaitingRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(meta = (BindWidget))
    UButton* OkButton;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* NicknameTextBox;


    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnStartButtonClicked();

protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
};
