// Fill out your copyright notice in the Description page of Project Settings.

#include "ElvenRingOnline.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineSessionInterface.h"

void UElvenRingOnline::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Display, TEXT("ElvenRingOnline Initialize"));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("ElvenRingOnline Initialize"));
	if (IOnlineSubsystem * OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *OnlineSubsystem->GetSubsystemName().ToString());
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, *OnlineSubsystem->GetSubsystemName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found"));
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("No Online Subsystem found"));
	}
}

void UElvenRingOnline::Deinitialize()
{
	Super::Deinitialize();
}

void UElvenRingOnline::CreateSession(int32 MaxPlayers, const FOnElvenRingCreateSessionComplete& InCallback)
{
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.NumPublicConnections = MaxPlayers;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bIsDedicated = false;

			if (IsSteamOnlineSubsystem(OnlineSubsystem))
			{
				UE_LOG(LogTemp,Display, TEXT("Non-LAN Online Subsystem"));
				SessionSettings.bAllowJoinInProgress = true;
				SessionSettings.bAllowJoinViaPresence = true;
				SessionSettings.bUseLobbiesIfAvailable = true;
				SessionSettings.bUsesPresence = true;
				SessionSettings.bIsLANMatch = false;
			}
			else
			{
				// Host가 맵을 이동해 버리면 In Progress가 필요하다.
				SessionSettings.bAllowJoinInProgress = true;
				// SessionSettings.bAllowJoinViaPresence = true;
				UE_LOG(LogTemp,Display, TEXT("LAN Online Subsystem"));
				SessionSettings.bIsLANMatch = true;
			}

			OnCreateSessionCompleteDelegate = InCallback;
			Session->ClearOnCreateSessionCompleteDelegates(this);
			Session->OnCreateSessionCompleteDelegates.AddUObject(this, &UElvenRingOnline::OnCreateSessionComplete);

			FUniqueNetIdPtr UserId = GetGameInstance()->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
			if (UserId.IsValid())
			{
				Session->CreateSession(*UserId, NAME_GameSession, SessionSettings);
			}
		}
	}
}

void UElvenRingOnline::FindSession(const FOnElvenRingFindSessionComplete& InCallback)
{
	UE_LOG(LogTemp, Display, TEXT("SearchSession"));
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("SearchSession"));

	if (IOnlineSubsystem * OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			OnElvenRingFindSessionCompleteDelegate = InCallback;
			Session->ClearOnFindSessionsCompleteDelegates(this);
			Session->OnFindSessionsCompleteDelegates.AddUObject(this, &UElvenRingOnline::OnFindSessionsComplete);

			SearchObject = MakeShareable(new FOnlineSessionSearch());
			SearchObject->MaxSearchResults = 10;

			if (IsSteamOnlineSubsystem(OnlineSubsystem))
			{
				UE_LOG(LogTemp, Display, TEXT("Search Steam Online Subsystem"));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Search Steam Online Subsystem"));
				SearchObject->bIsLanQuery = false;
				SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
				SearchObject->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Search LAN Online Subsystem"));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Search LAN Online Subsystem"));
				SearchObject->bIsLanQuery = true;
			}

			FUniqueNetIdPtr UserId = GetGameInstance()->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
			Session->FindSessions(*UserId, SearchObject.ToSharedRef());
		}
	}
}

void UElvenRingOnline::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnElvenRingJoinSessionComplete.IsBound())
	{
		OnElvenRingJoinSessionComplete.Execute(SessionName, Result);
	}

	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			Session->ClearOnJoinSessionCompleteDelegates(this);
			OnElvenRingJoinSessionComplete.Unbind();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found"));
	}

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session"));
		return;
	}

	FString ConnectString;
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			if (Session->GetResolvedConnectString(NAME_GameSession, ConnectString))
			{
				APlayerController* PlayerController = GetGameInstance()->GetFirstGamePlayer()->GetPlayerController(GetWorld());
				if (PlayerController)
				{
					PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
					UE_LOG(LogTemp,Display,TEXT("Connect String: %s"), *ConnectString);
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("Join Session Success"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("PlayerController is null"));
					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("PlayerController is null"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get resolved connect string"));
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("Failed to get resolved connect string"));
			}
		}
	}
}

void UElvenRingOnline::JoinSession(const FOnlineSessionSearchResult& SearchResult, const FOnElvenRingJoinSessionComplete& InCallback)
{
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			OnElvenRingJoinSessionComplete = InCallback;
			Session->ClearOnJoinSessionCompleteDelegates(this);
			Session->OnJoinSessionCompleteDelegates.AddUObject(this, &UElvenRingOnline::OnJoinSessionComplete);

			FUniqueNetIdPtr UserId = GetGameInstance()->GetFirstGamePlayer()->GetPreferredUniqueNetId().GetUniqueNetId();
			if (UserId.IsValid())
			{
				// https://forums.unrealengine.com/t/ue-5-5-online-subsystem-join-session-always-results-in-on-failure/2125579/14
				FOnlineSessionSearchResult ModSearchResult = SearchResult;
				ModSearchResult.Session.SessionSettings.bUsesPresence = true;
				ModSearchResult.Session.SessionSettings.bUseLobbiesIfAvailable = true;
				Session->JoinSession(*UserId, NAME_GameSession, ModSearchResult);
			}
		}
	}
}

void UElvenRingOnline::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnCreateSessionCompleteDelegate.IsBound())
	{
		OnCreateSessionCompleteDelegate.Execute(SessionName, bWasSuccessful);
	}
	
	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			Session->ClearOnCreateSessionCompleteDelegates(this);
			OnCreateSessionCompleteDelegate.Unbind();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found"));
	}
}

void UElvenRingOnline::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (OnElvenRingFindSessionCompleteDelegate.IsBound())
	{
		OnElvenRingFindSessionCompleteDelegate.Execute(bWasSuccessful, SearchObject->SearchResults);
	}
	UE_LOG(LogTemp,Display, TEXT("Found : %d"), SearchObject->SearchResults.Num());
	UE_LOG(LogTemp,Display, TEXT("Is Successful : %d"), bWasSuccessful);

	if (IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld()))
	{
		if (IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface())
		{
			Session->ClearOnFindSessionsCompleteDelegates(this);
			OnElvenRingFindSessionCompleteDelegate.Unbind();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found"));
	}
}

bool UElvenRingOnline::IsSteamOnlineSubsystem(const class IOnlineSubsystem* Subsystem)
{
	return Subsystem && Subsystem->GetSubsystemName() == "Steam";
}
