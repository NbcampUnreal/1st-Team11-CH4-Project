#include "WaitingRoomPlayerCardsRT.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SceneCapture2D.h"
#include "Camera/CameraComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "ElvenRing/UI/PlayerCard.h"
#include "ElvenRing/UI/WaitingRoomGameState.h"
#include "ElvenRing/UI/WaitingRoomPlayerState.h"
#include "ElvenRing/UI/WaitingRoomPlayerController.h"
#include  "WaitingRoomGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/WaitingRoomWidget.h"

AWaitingRoomPlayerCardsRT::AWaitingRoomPlayerCardsRT()
{
	if (bTempInit)
		return;
	bTempInit = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(RootComponent);
	SceneCapture->SetRelativeLocation(FVector(0, 200, 0));
	SceneCapture->SetRelativeRotation(FRotator(0, -90, 0));

	static ConstructorHelpers::FClassFinder<APlayerCard> BP_PlayerCard(TEXT("/Game/ElvenRing/Blueprints/UI/BP_PlayerCard"));//.BP_PlayerCard_C
	if (BP_PlayerCard.Succeeded())
	{
		PlayerCardClass = BP_PlayerCard.Class;
		UE_LOG(LogTemp, Warning, TEXT("PlayerCardClass: %s"), *GetNameSafe(PlayerCardClass));
	}
	SceneCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SceneCamera"));
	SceneCamera->SetupAttachment(RootComponent);
	SceneCamera->SetRelativeLocation( FVector(0.f, 200.f, 0.f));
	SceneCamera->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
}
void AWaitingRoomPlayerCardsRT::SetGlowPoworDir(float Dir,int32 Index)
{
	PlayerCards[Index]->CloseGlowPowr();
}
void AWaitingRoomPlayerCardsRT::OnUpdatePlayerName(int idx, FString Name)
{
	PlayerCards[idx]->SetName(FText::FromString( Name) );

	//UE_LOG(LogTemp, Warning, TEXT("PlayerCard CurInputName from Server: %s"), *PlayerCards[idx]->GetPlayerName().ToString());
}
UTextureRenderTarget2D* AWaitingRoomPlayerCardsRT::GetRenderTarget()
{
	return MyRenderTarget;
}
void AWaitingRoomPlayerCardsRT::SetName(FText Name)
{
	AWaitingRoomPlayerController* WaitingRoomPlayerController = Controller;//Cast<AWaitingRoomPlayerController>();//(UGameplayStatics::GetPlayerController(this, 0));
	if (!WaitingRoomPlayerController) return;

	//if( Controller->IsLocalController())
		WaitingRoomPlayerController->Server_SetName1(Name.ToString());
	

	/*int32 MyPlayerCardIndex = WaitingRoomPlayerController->GetMyPlayerIndex();
	UE_LOG(LogTemp, Warning, TEXT("MyPlayerCardIndex = %d"), MyPlayerCardIndex);
	if (MyPlayerCardIndex < 0 || MyPlayerCardIndex >= PlayerCards.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCard Index"));
		return;
	}
	PlayerCards[MyPlayerCardIndex]->SetName(Name);*/
}

void AWaitingRoomPlayerCardsRT::BeginPlay()
{
	Super::BeginPlay();

	MyRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	MyRenderTarget->InitAutoFormat(1920, 1080);
	MyRenderTarget->ClearColor = FLinearColor::Transparent;
	MyRenderTarget->UpdateResourceImmediate(true);

	SceneCapture->TextureTarget = MyRenderTarget;
	//UE_LOG(LogTemp, Warning, TEXT("AWaitingRoomPlayerCardsRT MyRenderTarget"));
	SceneCapture->TextureTarget->ClearColor = FLinearColor(0, 0, 0, 0);

	AWaitingRoomPlayerController* PC = Cast<AWaitingRoomPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(this, 0.f);
		
	}


	//SceneCapture->ShowFlags.SetLighting(false);
	//SceneCapture->ShowFlags.SetPostProcessing(false);
	//SceneCapture->ShowFlags.SetShadowFrustums(false);
	//SceneCapture->ShowFlags.SetFog(false);
	//SceneCapture->ShowFlags.SetAtmosphere(false);

	//SceneCapture->ShowFlags.SetBloom(true);
	//SceneCapture->ShowFlags.SetPostProcessing(true);
	//SceneCapture->ShowFlags.SetTonemapper(true);

	//SceneCapture->PostProcessSettings.bOverride_BloomIntensity = true;
	//SceneCapture->PostProcessSettings.BloomIntensity = 3.f;

	//SceneCapture->PostProcessSettings.bOverride_BloomThreshold = true;
	//SceneCapture->PostProcessSettings.BloomThreshold = -1.f;

	//SceneCapture->PostProcessSettings.bOverride_AutoExposureMethod = true;
	//SceneCapture->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
	//SceneCapture->PostProcessSettings.ExposureCompensationSettings.ExposureCompensation = 1.0f;
	//SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;

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
	//TempCreatePlayerCard();
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
					//UE_LOG(LogTemp, Warning, TEXT("Index %d / ElapsedTime = %f"), Index, ElapsedTime);
				}
				//}
			}
			else
				PrveTime = GetWorld()->GetTimeSeconds();
		}), 0.1f, true
	);
}

//네트워크 연결시 플레이어 카드 생성 예비 코드
void AWaitingRoomPlayerCardsRT::ConnectOpenPlayerCard(int32 Index,bool bMyOrder)
{
	int StartIdx = PlayerCards.Num();

	AWaitingRoomPlayerController* WaitingRoomPlayerController = Cast<AWaitingRoomPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (WaitingRoomPlayerController)
	{
		int32 ControllerIdx = WaitingRoomPlayerController->GetMyPlayerIndex();
		UE_LOG(LogTemp, Warning, TEXT("StartIdx   %d/ Index %d / ControllerIdx %d"), StartIdx, Index, ControllerIdx);
	}
	
	AWaitingRoomGameState* GameState = Cast<AWaitingRoomGameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error!!GameState") );
		return;
	}

	for (int32 i = StartIdx; i < Index;++i)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector SpawnLocation = FVector(-1025.f + 1025.f * i, -1500.f, 50.f);
		FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);
		
		APlayerCard* CreatePlayerCard = GetWorld()->SpawnActor<APlayerCard>(
			PlayerCardClass, SpawnLocation, SpawnRotation, SpawnParams );
		if (CreatePlayerCard)
		{
			CreatePlayerCard->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CreatePlayerCard->SetIndex(i);

			if (i < GameState->PlayerArray.Num())
			{
				AWaitingRoomPlayerState* PlayerState = Cast<AWaitingRoomPlayerState>(GameState->PlayerArray[i]);
				if (PlayerState)
				{
					FString PlayerName = PlayerState->PlayerNickName;
					CreatePlayerCard->SetName(FText::FromString(PlayerName));
					UE_LOG(LogTemp, Warning, TEXT("CreatePlayerCard %d  / PlayerName : %s"), i, *PlayerName);
				}
				PlayerCards.Add(CreatePlayerCard);
			}
		}
	}
}


