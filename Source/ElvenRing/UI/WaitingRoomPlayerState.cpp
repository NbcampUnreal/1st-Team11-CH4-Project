#include "WaitingRoomPlayerState.h"
#include <Net/UnrealNetwork.h>
#include "ElvenRing/UI/WaitingRoomPlayerController.h"

AWaitingRoomPlayerState::AWaitingRoomPlayerState()
{
	PlayerNickName = TEXT("Empty");//FText::FromString(TEXT("Empty"));
}
void AWaitingRoomPlayerState::Server_UpdatePlayerNickName(const FString& Name)//_Implementation
{
	PlayerNickName = Name;

	if (HasAuthority()) 
	{
		for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(It->Get()))
			{
				if (PC->IsLocalController())
					PC->Client_OnUpdatePlayerName(); // 오직 호스트 자기화면
			}
		}
	}
}
void AWaitingRoomPlayerState::OnRep_PlayerNickName()
{
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(It->Get()))
		{
			if (PC->IsLocalController())
				PC->Client_OnUpdatePlayerName(); // 오직 호스트 자기화면
		}
	}
	/*AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(GetPlayerController());
	if (PC) 
	{
		PC->Client_OnUpdatePlayerName();
	}*/

	//for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	//{
	//	if (AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(It->Get()))
	//	{
	//		if (PC->IsLocalController() )
	//		{
	//			PC->Client_OnUpdatePlayerName(); // 오직 호스트 자기화면
	//		}
	//	}
	//}
	//bUpdatePlayerName = !bUpdatePlayerName;
}

void AWaitingRoomPlayerState::BeginPlay()
{
	Super::BeginPlay();

    FTimerHandle TimerHandle;
	//GetWorld()->GetTimerManager().SetTimer
	//(
	//	TimerHandle,
	//	FTimerDelegate::CreateLambda([this]() mutable
	//	{
 //    /*       APlayerController* PC = GetPlayerController();
 //           if (!PC || !PC->IsLocalController())
 //           {
 //               UE_LOG(LogTemp, Warning, TEXT("Not LocalController / AWaitingRoomPlayerState"));
 //               return;
 //           }*/
 //           if (HasAuthority())
 //               PlayerName = FText::FromString(TEXT("Host"));
 //           else
 //               PlayerName = FText::FromString(TEXT("Guest"));
	//	}), 0.1f, false
	//);
}

void AWaitingRoomPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWaitingRoomPlayerState, PlayerNickName);
}