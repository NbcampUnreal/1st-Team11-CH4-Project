﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenringGameInstance.h"

#include "Blueprint/UserWidget.h"

void UElvenringGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UElvenringGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UElvenringGameInstance::OnPostLoadMap);
}

void UElvenringGameInstance::ShowLoadingScreen()
{
	UE_LOG(LogTemp, Display, TEXT("ShowLoadingScreen"));
	if (!LoadingScreenWidget && LoadingScreenClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenClass);
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->AddToViewport(9999);
		}
	}
}

void UElvenringGameInstance::HideLoadingScreen()
{
	if (LoadingScreenWidget)
	{
		LoadingScreenWidget->RemoveFromParent();
		LoadingScreenWidget = nullptr;
	}
}

void UElvenringGameInstance::OnPreLoadMap(const FString& String)
{
	UE_LOG(LogTemp, Display, TEXT("OnPreLoadMap: %s"), *String);
	ShowLoadingScreen();
}

void UElvenringGameInstance::OnPostLoadMap(UWorld* World)
{
	UE_LOG(LogTemp, Display, TEXT("OnPostLoadMap: %s"), *World->GetName());
	HideLoadingScreen();
}
