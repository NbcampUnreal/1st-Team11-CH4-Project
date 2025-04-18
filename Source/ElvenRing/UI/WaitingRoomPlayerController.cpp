// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitingRoomPlayerController.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/WaitingRoomPlayerCardsRT.h"
#include "ElvenRing/UI/WaitingRoomGameState.h"
#include "ElvenRing/UI/WaitingRoomWidget.h"
#include "ElvenRing/UI/WaitingRoomPlayerState.h"
#include <Net/UnrealNetwork.h>

AWaitingRoomPlayerController::AWaitingRoomPlayerController()
{
}

void AWaitingRoomPlayerController::SetIndex_Implementation(int32 Index)
{
	 MyPlayerIndex = Index; 
	 UE_LOG(LogTemp, Warning, TEXT("MyPlayerIndex %d"), MyPlayerIndex);
	 
	 //UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	 //if (EGameInstance)
	 //{
		// if (MyPlayerIndex == 0)
		// {
		//	 EGameInstance->GetUIManager()->GetWaitingRoomUi()->HostMode();
		// }
		// else if (MyPlayerIndex > 0)
		// {
		//	 EGameInstance->GetUIManager()->GetWaitingRoomUi()->GuestMode();
		// }
	 //}
	 //FString Path = TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT.BP_WaitingRoomPlayerCardsRT_C");
	 //UClass* WaitingRoomPlayerCardsRTClass = StaticLoadClass(AWaitingRoomPlayerCardsRT::StaticClass(), nullptr, *Path);
	 //if (WaitingRoomPlayerCardsRTClass)
	 //{
		// FActorSpawnParameters SpawnParams;
		// SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
		// FRotator Rotation = FRotator::ZeroRotator;
		// WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
	 //}

	 //EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
	 //// UE_LOG(LogTemp, Warning, TEXT("BeginPlay OPEN WaitingRoomPlayerCardsRT / MyPlayerIndex = %d"), MyPlayerIndex);
	 //EGameInstance->GetUIManager()->GetWaitingRoomUi()->SetOwningPlayer(this);
}

void AWaitingRoomPlayerController::OpenWaitingRoom_Implementation(int CurConnectedPlayerCountAndMyPlayerIdx)
{
	//if (!IsLocalController())
	//	return;
	
	if (!IsValid(WaitingRoomPlayerCardsRT)) 
	{
		TempCreateRT();
		UE_LOG(LogTemp, Warning, TEXT("NULL!!! WaitingRoomPlayerCardsRT"));
		//return;
	}
	else
	{
		WaitingRoomPlayerCardsRT->ConnectOpenPlayerCard(CurConnectedPlayerCountAndMyPlayerIdx);
	}
}
void AWaitingRoomPlayerController::Client_OnUpdatePlayerName_Implementation()
{
	//if (true)
	//	return;

	AWaitingRoomGameState* WaitingRoomGameState = Cast<AWaitingRoomGameState>(GetWorld()->GetGameState());
	if (WaitingRoomGameState)
	{
		//const TArray<APlayerState*> PlayerStates = WaitingRoomGameState->PlayerArray;

		//UE_LOG(LogTemp, Warning, TEXT("MyPlayerIndex = %d / PlayerArray.SIZE %d"), MyPlayerIndex, PlayerStates.Num());
		//UE_LOG(LogTemp, Warning, TEXT("GS->PlayerNames.Num = %d / PlayerArray.SIZE %d"), WaitingRoomGameState->PlayerNames.Num(), PlayerStates.Num());
		//if (WaitingRoomGameState->PlayerNames.Num() < PlayerStates.Num())
		//{
		//	// 아직 동기화가 안된 상태니까 타이머로 재시도
		//	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AWaitingRoomPlayerController::Client_OnUpdatePlayerName);
		//	UE_LOG(LogTemp, Warning, TEXT("Return PlayerNames.Num() < PlayerStates.Num()"));
		//	return;
		//}else
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Ok"));
		//}
		
		for (int32 i = 0; i < WaitingRoomGameState->PlayerNames.Num();++i)//(int32 i = 0; i < PlayerStates.Num();++i) //(APlayerState* PS : PlayerStates)
		{
			//FText PlayerCardName = Cast<AWaitingRoomPlayerState>(PlayerStates[i])->PlayerName;
			FText PlayerCardName = WaitingRoomGameState->PlayerNames[i];
			FString DBG = PlayerCardName.ToString();
			UE_LOG(LogTemp, Warning, TEXT("MyPlayerIndex = %d /CarIndex %d / PlayerCardName %s"), MyPlayerIndex,i, *DBG);
			WaitingRoomPlayerCardsRT->OnUpdatePlayerName(i, PlayerCardName );

		//	UE_LOG(LogTemp, Warning, TEXT("MyPlayerIndex = %d / PlayerCardName %s"), MyPlayerIndex, *PlayerCardName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WaitingRoomGameState is NULL"));
	}
}
void AWaitingRoomPlayerController::TempCreateRT()
{
	FString Path = TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT.BP_WaitingRoomPlayerCardsRT_C");
	UClass* WaitingRoomPlayerCardsRTClass = StaticLoadClass(AWaitingRoomPlayerCardsRT::StaticClass(), nullptr, *Path);
	if (WaitingRoomPlayerCardsRTClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
		FRotator Rotation = FRotator::ZeroRotator;
		WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
	}

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
		EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
//	UE_LOG(LogTemp, Warning, TEXT(" OPEN WaitingRoomPlayerCardsRT / MyPlayerIndex = %d"), MyPlayerIndex);
}
void AWaitingRoomPlayerController::Server_SetName1_Implementation(const FText& Name)
{

	//if (PS)
	//{
	//	//if (IsLocalController())
	//	{
	//		PS->PlayerName = Name;
	//		UE_LOG(LogTemp, Warning, TEXT(" PS->PlayerName %s / MyPlayerIndex = %d"), *PS->PlayerName.ToString(), MyPlayerIndex);
	//	}
	//}
	AWaitingRoomGameState* GS = GetWorld()->GetGameState<AWaitingRoomGameState>();
	if (GS)
	{
		Cast<AWaitingRoomPlayerState>(GS->PlayerArray[MyPlayerIndex])->PlayerName = Name;
		AWaitingRoomPlayerState* PS = Cast<AWaitingRoomPlayerState>(PlayerState);

		GS->UpdatePlayerName(Name);
		//GS->bUpdatePlayerName = !GS->bUpdatePlayerName; // RepNotify 트리거
		//if (HasAuthority())
		//{
		//	GS->OnRep_bUpdatePlayerName();
		//	UE_LOG(LogTemp, Warning, TEXT(" HasAuthority  Server_SetName1_Implementation"));
		//}
		//else
		//{
		//	GS->bUpdatePlayerName = GS->bUpdatePlayerName ? false : true; //
		//	UE_LOG(LogTemp, Warning, TEXT(" CLIENT  Server_SetName1_Implementation"));
		//}
	}

	
}
void AWaitingRoomPlayerController::BeginPlay()
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not LocalController / MyPlayerIndex : %d"), MyPlayerIndex);
		return;
	}

	UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	if (EGameInstance)
	{
		if (HasAuthority())//(MyPlayerIndex == 0)
		{
			EGameInstance->GetUIManager()->GetWaitingRoomUi()->HostMode();
		}
		else //if (MyPlayerIndex > 0)
		{
			EGameInstance->GetUIManager()->GetWaitingRoomUi()->GuestMode();
		}
	}
	FString Path = TEXT("/Game/ElvenRing/Blueprints/UI/BP_WaitingRoomPlayerCardsRT.BP_WaitingRoomPlayerCardsRT_C");
	UClass* WaitingRoomPlayerCardsRTClass = StaticLoadClass(AWaitingRoomPlayerCardsRT::StaticClass(), nullptr, *Path);
	if (WaitingRoomPlayerCardsRTClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location(0, 0, 100); // 카메라가 보기에 적당한 위치
		FRotator Rotation = FRotator::ZeroRotator;
		WaitingRoomPlayerCardsRT = GetWorld()->SpawnActor<AWaitingRoomPlayerCardsRT>(WaitingRoomPlayerCardsRTClass, Location, Rotation, SpawnParams);
		WaitingRoomPlayerCardsRT->Controller = this;
	}

	EGameInstance->GetUIManager()->ShowWaitingRoom(GetWorld(), WaitingRoomPlayerCardsRT);
	// UE_LOG(LogTemp, Warning, TEXT("BeginPlay OPEN WaitingRoomPlayerCardsRT / MyPlayerIndex = %d"), MyPlayerIndex);
	EGameInstance->GetUIManager()->GetWaitingRoomUi()->SetOwningPlayer(this);


	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT(" Has Authority "), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" No Authority"), *GetName());
	}

	//switch (GetNetMode())
	//{
	//case NM_Standalone:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: Standalone"));
	//	break;
	//case NM_ListenServer:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: Listen Server"));
	//	break;
	//case NM_DedicatedServer:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: Dedicated Server"));
	//	break;
	//case NM_Client:
	//	UE_LOG(LogTemp, Warning, TEXT("NetMode: Client"));
	//	break;
	//}
}

void AWaitingRoomPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWaitingRoomPlayerController, MyPlayerIndex);
}
