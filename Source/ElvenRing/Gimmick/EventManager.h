// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFlagChanged, bool, bNewValue, bool, bOldValue);

/**
 * 
 */
UCLASS(BlueprintType)
class ELVENRING_API UEventManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(const TArray<class UGameFlag*>* InGameFlagAssets);	
	UFUNCTION(BlueprintCallable)
	void SetGameFlag(FName FlagName, bool bNewValue);
	UFUNCTION(BlueprintCallable)
	bool GetGameFlag(FName FlagName) const;

	// 현재 구현 상의 문제로 Delegate를 직접 얻어서 사용하고 있다.
	// 직접 접근해서 등록하고 해제하도록 한다.
	FOnFlagChanged* GetOnFlagChanged(FName FlagName);

protected:
	TMap<FName, bool> GameFlagStates;
	TMap<FName, FOnFlagChanged> Delegates;

private:
	UPROPERTY()
	TArray<class UGameFlag*> GameFlagAssets;
};
