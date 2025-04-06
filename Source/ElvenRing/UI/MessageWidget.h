// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidgetBase.h"
#include "MessageWidget.generated.h"

class UTextBlock;

UCLASS()
class ELVENRING_API UMessageWidget : public UMessageWidgetBase
{
	GENERATED_BODY()

public:

	// 자식인 나는 ShowMessageText()를 호출해야함. ShowMessageText_Implementation()를 호출하면 안된다. 
	virtual void ShowMessageText_Implementation(const FString& Message) override;

	//virtual void ShowMessageText(const FString& Message) override;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

};
