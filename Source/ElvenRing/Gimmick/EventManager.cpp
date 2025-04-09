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
			// Editor 모드에서 이렇게 동작시키면 Editor Asset의 값을 수정해버린다.
			// if (UGameFlag** GameFlag = GameFlagAssets.FindByPredicate([&FlagName](const UGameFlag* Flag) { return Flag->FlagName == FlagName; }))
			// {
			// 	UE_LOG(LogTemp, Display, TEXT("Find Flag in GameFlagAssets"));
			// 	(*GameFlag)->bDefaultValue = bNewValue;
			// }
			// else
			// {
			// 	UE_LOG(LogTemp,Display,TEXT("Event Manager: SetGameFlag: %s not found in GameFlagAssets"), *FlagName.ToString());
			// }

			// Delegate 호출
			if (FOnFlagChanged* Delegate = Delegates.Find(FlagName))
			{
				Delegate->Broadcast(bNewValue, *OldValue);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("Event Manager: SetGameFlag: %s not found"), *FlagName.ToString());
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
