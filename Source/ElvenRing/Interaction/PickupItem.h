// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

UCLASS()
class ELVENRING_API APickupItem : public AActor, public IInteractable
{
public:
	virtual FString GetInteractText() override;
	virtual void Interact(APlayerController* Player) override;
	virtual bool CanInteract() override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
