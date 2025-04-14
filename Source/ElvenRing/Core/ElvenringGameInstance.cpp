// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenringGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/Character/UnitBase.h"

UElvenringGameInstance::UElvenringGameInstance()
{
	static ConstructorHelpers::FClassFinder<UUIManager> BP_UIManager(TEXT("/Game/ElvenRing/Blueprints/UI/BP_UiManager"));
	if (BP_UIManager.Succeeded())
		UIManagerClass = BP_UIManager.Class;
}

void UElvenringGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UElvenringGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UElvenringGameInstance::OnPostLoadMap);

	if (UIManagerClass)
	{
		UIManager = NewObject<UUIManager>(this, UIManagerClass);
		UIManager->InitUi(GetWorld());
	}

}

void UElvenringGameInstance::ShowLoadingScreen(const FString& TargetMapName)
{
	if (!LoadingScreenWidget && LoadingScreenClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(this, LoadingScreenClass);
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->AddToViewport(9999);
		}
		StartTrackLoadProgress(TargetMapName);
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

void UElvenringGameInstance::OnPreLoadMap(const FString& MapName)
{
	StartTrackLoadProgress(MapName);
}

void UElvenringGameInstance::OnPostLoadMap(UWorld* World)
{
	UE_LOG(LogTemp, Display, TEXT("OnPostLoadMap: %s / Authority : %s"), *World->GetName(), World->GetNetMode() == NM_Standalone ? TEXT("True") : TEXT("False"));
	HideLoadingScreen();
	GetTimerManager().ClearTimer(LoadTimerHandle);
}

void UElvenringGameInstance::StartTrackLoadProgress(const FString& MapName)
{
	TargetMapPackageName = FName(*MapName);
	UE_LOG(LogTemp, Display, TEXT("StartTrackLoadProgress: %s"), *MapName);
	
	GetTimerManager().SetTimer(LoadTimerHandle, this, &UElvenringGameInstance::CheckLoadProgress, 0.01f, true);
	UE_LOG(LogTemp, Display, TEXT("Set Timer"));
}

void UElvenringGameInstance::CheckLoadProgress()
{
	// UE_LOG(LogTemp, Display, TEXT("Check Progress"));
	// if (TargetMapPackageName.IsNone()) return;
	
	// float Percentage = GetAsyncLoadPercentage(TargetMapPackageName);
	// UE_LOG(LogTemp, Display, TEXT("Loading %s: %.2f"), *TargetMapPackageName.ToString(), Percentage);
}

void UElvenringGameInstance::BindToCharacterOpenWidget(ECharacterType Type, AUnitBase* Unit)
{
	if (ECharacterType::Player == Type)
		UIManager->BindPlayerMainUi(GetWorld(), Unit);
	else if (ECharacterType::NormalMonster == Type)
		UIManager->CreateBindNormalMonsterWidgetUi(GetWorld(), Unit);
	else if (ECharacterType::Boss == Type)
		UIManager->BindBossWidgetUi(GetWorld(), Unit);
}
