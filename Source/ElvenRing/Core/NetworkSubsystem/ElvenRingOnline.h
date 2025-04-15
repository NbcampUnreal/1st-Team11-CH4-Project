// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ElvenRingOnline.generated.h"

DECLARE_DELEGATE_TwoParams(FOnElvenRingCreateSessionComplete, FName /* Session Name */, bool /* bWasSuccessful */);
DECLARE_DELEGATE_TwoParams(FOnElvenRingFindSessionComplete, bool /* bWasSuccessful */, const TArray<FOnlineSessionSearchResult>& /* SearchResults */);
DECLARE_DELEGATE_TwoParams(FOnElvenRingJoinSessionComplete, FName /* Session Name */, EOnJoinSessionCompleteResult::Type /* Result */);

/**
 * 
 */
UCLASS()
class ELVENRING_API UElvenRingOnline : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Session을 생성한다. Session을 생성하고 맵을 이동하지는 않는다. 생성 후에 InCallback을 호출한다. */
	void CreateSession(int32 MaxPlayers, const FOnElvenRingCreateSessionComplete& InCallback);
	/** 참여 가능한 Session을 찾는다. 완료 후에 InCallback을 호출한다. */
	void FindSession(const FOnElvenRingFindSessionComplete& InCallback);
	/** Join을 실행한다. Join이 성공하면 Callback 후에 맵을 이동한다. 만약에 Join에 실패하면 그대로 반환한다. */
	void JoinSession(const FOnlineSessionSearchResult& SearchResult, const FOnElvenRingJoinSessionComplete& InCallback);
	// void DestroySession();

protected:	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
private:
	FOnElvenRingCreateSessionComplete OnCreateSessionCompleteDelegate;
	TSharedPtr<class FOnlineSessionSearch> SearchObject;
	FOnElvenRingFindSessionComplete OnElvenRingFindSessionCompleteDelegate;
	FOnElvenRingJoinSessionComplete OnElvenRingJoinSessionComplete;
	
protected:
	static bool IsSteamOnlineSubsystem(const class IOnlineSubsystem* Subsystem);
};
