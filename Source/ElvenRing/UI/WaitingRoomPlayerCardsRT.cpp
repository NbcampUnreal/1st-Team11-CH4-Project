#include "WaitingRoomPlayerCardsRT.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Camera/CameraComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ElvenRing/UI/PlayerCard.h"

AWaitingRoomPlayerCardsRT::AWaitingRoomPlayerCardsRT()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->SetRelativeLocation(FVector(0, 200, 0));
	SceneCapture->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<APlayerCard> BP_PlayerCard(TEXT("/Game/ElvenRing/Blueprints/UI/BP_PlayerCard"));//.BP_PlayerCard_C
	if (BP_PlayerCard.Succeeded())
		PlayerCardClass = BP_PlayerCard.Class;
		UE_LOG(LogTemp, Warning, TEXT("PlayerCardClass: %s"), *GetNameSafe(PlayerCardClass));

	
}

void AWaitingRoomPlayerCardsRT::SetName(FText Name,int32 Idx)
{
	PlayerCards[Idx]->SetName(Name);
}

void AWaitingRoomPlayerCardsRT::BeginPlay()
{
	Super::BeginPlay();
	SceneCapture->TextureTarget->ClearColor = FLinearColor(0, 0, 0, 0);
	//SceneCapture->ShowFlags.SetLighting(false);
	SceneCapture->ShowFlags.SetPostProcessing(false);
	SceneCapture->ShowFlags.SetShadowFrustums(false);
	SceneCapture->ShowFlags.SetFog(false);
	SceneCapture->ShowFlags.SetAtmosphere(false);

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//for (int32 i = 0; i < 3; ++i)
	//{
	//	FVector SpawnLocation = FVector(-1100.f + 1100.f * Index, -1500.f, 50.f);
	//	FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
	//	APlayerCard* CreatePlayerCard = GetWorld()->SpawnActor<APlayerCard>(
	//		PlayerCardClass, SpawnLocation, SpawnRotation, SpawnParams
	//	);
	//	CreatePlayerCard->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//	if (CreatePlayerCard)
	//		CreatePlayerCard->SetName(FText::FromString(TEXT("Guest")));
	//	PlayerCards.Add(CreatePlayerCard);
	//}
	TempCreatePlayerCard();
}
void AWaitingRoomPlayerCardsRT::TempCreatePlayerCard()
{
	float ElapsedTime = 0.f;
	int32 Index = 0;
	float PrveTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle,
		FTimerDelegate::CreateLambda([this, Index, ElapsedTime, PrveTime]() mutable
			{
				ElapsedTime += GetWorld()->GetTimeSeconds() - PrveTime;
				if (ElapsedTime > 0.8f)
				{
					ElapsedTime = 0.f;
					FActorSpawnParameters SpawnParams;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					//for (int32 i = 0; i < 3; ++i)
					//{
					FVector SpawnLocation = FVector(-1025.f + 1025.f * Index, -1500.f, 50.f);
					FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
					APlayerCard* CreatePlayerCard = GetWorld()->SpawnActor<APlayerCard>(
						PlayerCardClass, SpawnLocation, SpawnRotation, SpawnParams
					);

					if (CreatePlayerCard)
					{
						CreatePlayerCard->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
						CreatePlayerCard->SetName(FText::FromString(TEXT("Guest")));
						CreatePlayerCard->SetIndex(Index);
						PlayerCards.Add(CreatePlayerCard);
					}
					Index++;
					if (Index >= 3)
					{
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
						UE_LOG(LogTemp, Warning, TEXT("Index %d / ElapsedTime = %f"), Index, ElapsedTime);
					}
					//}
				}
				else
					PrveTime = GetWorld()->GetTimeSeconds();
			}), 0.1f, true
	);
}

