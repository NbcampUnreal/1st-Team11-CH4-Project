// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTypes.generated.h" 


USTRUCT(BlueprintType)
struct FRenderTargetMaterialSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* RenderTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic*	Material;
};
USTRUCT(BlueprintType)
struct FResultStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageDealt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DamageTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RespawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RollCount;
};

UENUM(BlueprintType)
enum class EResultStat : uint8
{
	DamageDealt = 0,
	DamageTaken = 1,
	RespawnCount = 2,
	RollCount = 3
};


//class ELVENRING_API CommonTypes
//{
//public:
//	CommonTypes();
//	~CommonTypes();
//};
