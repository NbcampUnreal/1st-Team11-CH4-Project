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

	if (UIManagerClass) //ksw
	{
		UIManager = NewObject<UUIManager>(this, UIManagerClass);
		UIManager->InitUi(GetWorld());
	}
}

void UElvenringGameInstance::ShowLoadingScreen()
{
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
	ShowLoadingScreen();
}

void UElvenringGameInstance::OnPostLoadMap(UWorld* World)
{
	HideLoadingScreen();
}
void UElvenringGameInstance::BindToCharacterOpenWidget(ECharacterType Type, AUnitBase* Unit)
{
	if (ECharacterType::Player == Type)
		UIManager->BindPlayerMainUi(Unit);
	else if (ECharacterType::NormalMonster == Type)
		UIManager->CreateBindNormalMonsterWidgetUi(Unit);
	else if (ECharacterType::Boss == Type)
		UIManager->BindBossWidgetUi(Unit);
}