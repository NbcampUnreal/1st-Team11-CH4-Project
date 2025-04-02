// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterWidget.generated.h"

class UProgressBar;
class UImage;
UCLASS()
class ELVENRING_API UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* HpFrameImg;

	UFUNCTION(BlueprintCallable, Category =  "UI")
	void SetSize(float size);
	void UpdateHpBar(float Hp);
};
