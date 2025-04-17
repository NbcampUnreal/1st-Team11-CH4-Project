// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "Elvenring/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"

ATitlePlayerController::ATitlePlayerController()
{

}

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not LocalController / ATitlePlayerController"));
		return;
	}

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		EGameInstance->GetUIManager()->ShowTitleScreen(GetWorld());
	}
}
