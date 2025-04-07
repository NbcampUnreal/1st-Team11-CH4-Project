// Fill out your copyright notice in the Description page of Project Settings.


#include "EventManager.h"

#include "GameFlag.h"

void UEventManager::Init(const TArray<class UGameFlag*>* InGameFlagAssets)
{
	GameFlagAssets = *InGameFlagAssets;
	
	// Game Flag Assets를 이용해서 map 초기화
	for (const UGameFlag* GameFlagAsset : GameFlagAssets)
	{
		if (GameFlagAsset != nullptr)
		{
			GameFlagStates.Add(GameFlagAsset->FlagName, GameFlagAsset->bDefaultValue);
		}
	}
}

void UEventManager::SetGameFlag(FName FlagName, bool bNewValue)
{
	if (const bool* OldValue = GameFlagStates.Find(FlagName))
	{
		if (*OldValue != bNewValue)
		{
			GameFlagStates[FlagName] = bNewValue;
			if (UGameFlag** GameFlag = GameFlagAssets.FindByPredicate([&FlagName](const UGameFlag* Flag) { return Flag->FlagName == FlagName; }))
			{
				(*GameFlag)->bDefaultValue = bNewValue;
			}

			// Delegate 호출
			if (FOnFlagChanged* Delegate = Delegates.Find(FlagName))
			{
				Delegate->Broadcast(bNewValue, *OldValue);
			}
		}
	}
}

bool UEventManager::GetGameFlag(FName FlagName) const
{
	if (const bool* Value = GameFlagStates.Find(FlagName))
	{
		return *Value;
	}
	return false;
}

FOnFlagChanged* UEventManager::GetOnFlagChanged(const FName FlagName)
{
	if (GameFlagStates.Contains(FlagName))
	{
		return &Delegates.FindOrAdd(FlagName);
	}
	return nullptr;
}
