// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidgetBase.h"
#include "BattleMessageWidget.generated.h"

UCLASS()
class ELVENRING_API UBattleMessageWidget : public UMessageWidgetBase
{
	GENERATED_BODY()
	
public:
	virtual void ShowMessageText_Implementation(const FString& Message) override;
	//virtual void ShowMessageText(const FString& Message) override;
protected:
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:

};
