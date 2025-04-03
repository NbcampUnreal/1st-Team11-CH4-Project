// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingController.h"
#include "EnhancedInputSubsystems.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/Core/ElvenRingGameMode.h"

AElvenRingController::AElvenRingController()
	: InputMappingContext(nullptr),
	  MoveAction(nullptr),
	  JumpAction(nullptr),
	  LookAction(nullptr),
	  SprintAction(nullptr),
	  DodgeAction(nullptr),
	  AttackAction(nullptr),
	  DefenceAction(nullptr),
	  EndDefenceAction(nullptr)

{
	
}
void AElvenRingController::BeginPlay()
{
	Super::BeginPlay();

	// 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		// Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				// Subsystem을 통해 우리가 할당한 IMC를 활성화
				// 우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void AElvenRingController::ServerRequestLevelChange_Implementation(const FString& LevelPath)
{
	MulticastShowLoadingScreen();
	
	if (AElvenRingGameMode* GameMode =  GetWorld()->GetAuthGameMode<AElvenRingGameMode>())
	{
		GameMode->HandleLevelTransition(this, LevelPath);
	}
}

void AElvenRingController::MulticastShowLoadingScreen_Implementation()
{
	if (UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
	{
		GameInstance->ShowLoadingScreen();
	}
}