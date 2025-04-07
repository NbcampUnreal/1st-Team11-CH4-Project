// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ElvenRing/Character/UnitBase.h"
#include "ElvenringGameInstance.generated.h"

class UUIManager;
class AUnitBase;

UCLASS()
class ELVENRING_API UElvenringGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UElvenringGameInstance();
	virtual void Init() override;

	UFUNCTION(Blueprintable)
	void ShowLoadingScreen();
	UFUNCTION(BlueprintCallable)
	void HideLoadingScreen();

private:
	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingScreenWidget;;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadingScreenClass;

	UFUNCTION()
	void OnPreLoadMap(const FString& String);
	UFUNCTION()
	void OnPostLoadMap(UWorld* World);



public://ksw
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUIManager* GetUIManager() const { return UIManager; };

	UFUNCTION(BlueprintCallable, Category = "Game")
	FString GetNickname() const { return Nickname; };

	UFUNCTION(BlueprintCallable, Category = "Game")
	void SetNickname(const FString& NewName) { Nickname = NewName; }

	void BindToCharacterOpenWidget(ECharacterType Type, AUnitBase* Unit);
private://ksw
	UPROPERTY()
	UUIManager* UIManager;

	UPROPERTY()
	TSubclassOf<UUIManager> UIManagerClass;

	UPROPERTY()
	FString Nickname;

public:
	TArray<class UGameFlag*>* GetGameFlags()
	{
		return &GameFlags;
	}
	
protected:
	// TO-DO : 추후에 데이터테이블로 변경
	UPROPERTY(EditDefaultsOnly)
	TArray<class UGameFlag*> GameFlags;
};
