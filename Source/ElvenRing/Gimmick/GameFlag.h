// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFlag.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ELVENRING_API UGameFlag : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName FlagName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDefaultValue;
};
