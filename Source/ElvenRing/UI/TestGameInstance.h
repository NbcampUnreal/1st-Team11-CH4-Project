// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TestGameInstance.generated.h"

class UMessageWidget;

UCLASS()
class ELVENRING_API UTestGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void ShowGlobalMessage(const FString& Message, float Duration = 2.f);

private:
	TSubclassOf<UUserWidget> MessageWidgetClass;
	UMessageWidget* MessageWidgetInstance = nullptr;
	FTimerHandle HideMessageTImerHandle;
};
