// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "LevelPortal.generated.h"

UCLASS()
class ELVENRING_API ALevelPortal : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelPortal();

	virtual FString GetInteractText() override;
	virtual void Interact(APlayerController* Player) override;
	virtual bool CanInteract() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString LevelPath;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
