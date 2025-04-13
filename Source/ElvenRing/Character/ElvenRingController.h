// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ElvenRingController.generated.h"

class UInputMappingContext; 
class UInputAction;
/**
 * 
 */
UCLASS()
class ELVENRING_API AElvenRingController : public APlayerController
{
	GENERATED_BODY()


public:
	AElvenRingController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	// IA_Move를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	// IA_Jump를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* JumpAction;
	// IA_Look를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;
	// IA_Sprint를 지정할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DefenceAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EndDefenceAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* InteractAction;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRequestLevelChange(const FString& LevelPath);
	UFUNCTION(Client, Reliable)
	void ClientShowLoadingScreen(const FString& TargetMapName);

	virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;
	/** Begin Play 시점에서 Ready 상태를 전달한다. */
	UFUNCTION(Server, Reliable)
	void ServerReportPlayerReady();
};
