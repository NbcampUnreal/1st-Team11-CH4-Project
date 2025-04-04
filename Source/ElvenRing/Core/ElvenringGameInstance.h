﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ElvenringGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ELVENRING_API UElvenringGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UFUNCTION(Blueprintable)
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingScreenWidget;;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingScreenClass;

	UFUNCTION()
	void OnPreLoadMap(const FString& String);
	UFUNCTION()
	void OnPostLoadMap(UWorld* World);
};
