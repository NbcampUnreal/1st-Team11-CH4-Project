// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItem.h"


FString APickupItem::GetInteractText()
{
	return TEXT("Pickup Item");
}

void APickupItem::Interact(APlayerController* Player)
{
}

bool APickupItem::CanInteract()
{
	return  false;
}

// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
	
	// Static Mesh 로드
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MeshAsset.Succeeded())
	{
	    Mesh->SetStaticMesh(MeshAsset.Object);
	}
	
	// Material 로드{
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
	if (MaterialAsset.Succeeded())
	{
	    Mesh->SetMaterial(0, MaterialAsset.Object);
	}
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

