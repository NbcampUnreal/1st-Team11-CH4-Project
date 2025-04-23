// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonTypes.h"
#include "ScorePageWidget.generated.h"

//USTRUCT(BlueprintType)
//struct FResultStat
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FString PlayerName;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 DamageDealt;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 DamageTaken;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 RespawnCount;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	int32 RollCount;
//};
//
//UENUM(BlueprintType)
//enum class EResultStat : uint8
//{
//	DamageDealt = 0,
//	DamageTaken = 1,
//	RespawnCount = 2,
//	RollCount = 3
//};

class UTextBlock;
class UButton;
UCLASS()
class ELVENRING_API UScorePageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextName1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextName2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextAttackValue1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextAttackValue2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDamageValue1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextDamageValue2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextReSpwanCount1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextReSpwanCount2;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextRollCount1;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextRollCount2;

	UPROPERTY(meta = (BindWidget))
	UButton* OkButton;

	UFUNCTION(BlueprintCallable,Category = "UI")
	void SetResultStat(int32 idx, EResultStat Stat, int Value);
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetName(int32 idx, FString Name);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnClickOkBtn();
	void CalculateResultStat();
	
protected:
	TArray<UTextBlock*> PlayerNames;
	TArray<FResultStat> AllResultStats;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
