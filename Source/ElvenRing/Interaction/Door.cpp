﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ADoor::ADoor()
	:DefaultLockInteractText(TEXT("문이 닫혀 있습니다")),
	DefaultOpenInteractText(TEXT("E: 문을 엽니다.")),
	DefaultCloseInteractText(TEXT("E: 문을 닫습니다."))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
	OpenTransform = FTransform::Identity;
	CloseTransform = FTransform::Identity;
	OpenCloseDuration = 1.0f;
	DoorMovementUpdateInterval = 0.1f;
	CurrentAlpha = 0.0f;

	bIsLocked = false;
	bIsOpen = false;
	bCanOpen = true;
	bCanClose = true;
	bReplicates = true;

	SetNetDormancy(DORM_DormantAll);
}

void ADoor::Open()
{
	if (IsLocked() || !bCanOpen)
	{
		return;
	}
	SetNetDormancy(DORM_Awake);
	bIsOpen = true;
	SetNetDormancy(DORM_DormantAll);
	StartDoorMovement();
	if (OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			OpenSound,
			GetActorLocation()
		);
	}
	UE_LOG(LogTemp,Display, TEXT("Door Opened / Authority : %s"), *FString(HasAuthority() ? TEXT("True") : TEXT("False")));
}

void ADoor::Close()
{
	if (!bCanClose)
	{
		return;
	}
	SetNetDormancy(DORM_Awake);
	bIsOpen = false;
	SetNetDormancy(DORM_DormantAll);
	StartDoorMovement();
	if (CloseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			CloseSound,
			GetActorLocation()
		);
	}
}

void ADoor::Toggle()
{
	if (bIsOpen)
	{
		Close();
	}
	else
	{
		Open();
	}
}

void ADoor::StartDoorMovement()
{
	GetWorldTimerManager().SetTimer(OpenCloseTimerHandle, this, &ADoor::UpdateDoorMovement, DoorMovementUpdateInterval, true);
}

void ADoor::UpdateDoorMovement()
{
	CurrentAlpha += (DoorMovementUpdateInterval / OpenCloseDuration) * (bIsOpen ? 1 : -1);

	if (CurrentAlpha >= 1.0f && bIsOpen)
	{
		CurrentAlpha = 1.0f;
		GetWorldTimerManager().ClearTimer(OpenCloseTimerHandle);
	}
	else if (CurrentAlpha <= 0.0f && !bIsOpen)
	{
		CurrentAlpha = 0.0f;
		GetWorldTimerManager().ClearTimer(OpenCloseTimerHandle);
	}

	FVector NewLocation = FMath::Lerp(CloseTransform.GetLocation(), OpenTransform.GetLocation(), CurrentAlpha);
	FRotator NewRotation = FMath::Lerp(CloseTransform.GetRotation().Rotator(), OpenTransform.GetRotation().Rotator(), CurrentAlpha);
	Mesh->SetRelativeLocationAndRotation(NewLocation, NewRotation);
}

void ADoor::OnRep_IsOpen()
{
	StartDoorMovement();
	UE_LOG(LogTemp,Display, TEXT("Door Opened / Authority : %s"), *FString(HasAuthority() ? TEXT("True") : TEXT("False")));
	if (bIsOpen && OpenSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			OpenSound,
			GetActorLocation()
		);
	}
	else if (!bIsOpen && CloseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			CloseSound,
			GetActorLocation()
		);
	}
}

FString ADoor::GetInteractText()
{
	if (IsLocked())
	{
		return LockInteractText.IsEmpty() ? DefaultLockInteractText : LockInteractText;
	}
	if (bIsOpen)
	{
		return CloseInteractText.IsEmpty() ? DefaultCloseInteractText : CloseInteractText;
	}
	else
	{
		return OpenInteractText.IsEmpty() ? DefaultOpenInteractText : OpenInteractText;
	}
}

void ADoor::Interact(APlayerController* Player)
{
	if (!HasAuthority())
	{
		return;
	}

	Toggle();
}

bool ADoor::CanInteract()
{
	return (bIsOpen && bCanClose) || (!bIsOpen && bCanOpen);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
}

#if WITH_EDITOR
void ADoor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ADoor, bIsOpen))
	{
		FVector TargetLocation = bIsOpen ? OpenTransform.GetLocation() : CloseTransform.GetLocation();
		FRotator TargetRotation = bIsOpen ? OpenTransform.GetRotation().Rotator() : CloseTransform.GetRotation().Rotator();
		Mesh->SetRelativeLocationAndRotation(TargetLocation, TargetRotation);
	}
}
#endif
