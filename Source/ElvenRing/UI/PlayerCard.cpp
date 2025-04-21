#include "PlayerCard.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ElvenRing/UI/PlayerNameWidget.h"
#include "Components/WidgetComponent.h"

APlayerCard::APlayerCard()
{
	BoneName = TEXT("Dummy");//DummySocket
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	PlayerCardMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	PlayerCardMesh->SetupAttachment(RootComponent);

	NameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	NameWidgetComponent->SetupAttachment(PlayerCardMesh, BoneName ); //idle 모션때문에 본에 붙여야 같이 움직인다.
	NameWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	//NameWidgetComponent->SetDrawSize(FVector2D(250.f, 50.f));
	NameWidgetComponent->SetTwoSided(true);

	static ConstructorHelpers::FClassFinder<UPlayerNameWidget> WBP_PlayerNameClass(TEXT("/Game/ElvenRing/Blueprints/UI/WBP_PlayerName"));
	if (WBP_PlayerNameClass.Succeeded())
	{
		NameWidgetComponent->SetWidgetClass(WBP_PlayerNameClass.Class);
	}
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickInterval = 0.02f; 
}

void APlayerCard::SetIndex(int32 Index)
{
	PlayerIndex = Index;
}

void APlayerCard::SetName(FText Name)
{
	UPlayerNameWidget* NickNameWidget = Cast<UPlayerNameWidget>(NameWidgetComponent->GetUserWidgetObject());
	NickNameWidget->SetName(Name);
}
FText APlayerCard::GetPlayerName()
{
	UPlayerNameWidget* NickNameWidget = Cast<UPlayerNameWidget>(NameWidgetComponent->GetUserWidgetObject());
	return NickNameWidget->GetTextName();
}
void APlayerCard::CloseGlowPowr()
{
	GlowPowerDir = -1.f;
	GlowPowerSpeed *= 1.25f;
	LightSweep();
}
void APlayerCard::BeginPlay()
{
	Super::BeginPlay();

	GlowPower = 0.f;
	bInit = true;
	//PlayerCardMesh->CreateDynamicMaterialInstance(0, PlayerCardMaterialInstance);
	PlayerCardDynamicMaterial = PlayerCardMesh->CreateAndSetMaterialInstanceDynamic(0);
	if(PlayerCardDynamicMaterial)
		PlayerCardDynamicMaterial->SetScalarParameterValue("Opacity", Opacity);
	int32 dsfsdf = 9;
}
void APlayerCard::LightSweep()
{
	float ElapsedTime = 0.f;
	float PrevTime = 0.f;
	TWeakObjectPtr<APlayerCard> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		LightSweepDelayTimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, ElapsedTime, PrevTime]() mutable
		{
			if (!SafeThis.IsValid())return;
			UWorld* World = SafeThis->GetWorld();
			if (!World) return;

			PrevTime = World->GetTimeSeconds();
			World->GetTimerManager().SetTimer
			(
				SafeThis->LightSweepTimerHandle,
				FTimerDelegate::CreateLambda([SafeThis, ElapsedTime, PrevTime]() mutable
				{
					if (!SafeThis.IsValid())return;
					UWorld* World = SafeThis->GetWorld();
					if (!World) return;
					ElapsedTime += World->GetTimeSeconds() - PrevTime;
					SafeThis->PlayerCardDynamicMaterial->SetScalarParameterValue("LightSweepElapsedTime", ElapsedTime);

					//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime %f / idx %d"), ElapsedTime, Index);
					if (ElapsedTime >= 2.1f)
					{
						SafeThis->PlayerCardDynamicMaterial->SetScalarParameterValue("LightSweepElapsedTime", 0.0f);
						World->GetTimerManager().ClearTimer(SafeThis->LightSweepTimerHandle);
					}
					else
						PrevTime = World->GetTimeSeconds();
				}),0.01f, true
			);

		}),1.f, false
	);
}
void APlayerCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bInit)return;

	if (NameWidgetComponent && PlayerCardMesh)
	{
		FVector BoneWorldLocation = PlayerCardMesh->GetBoneLocation(BoneName);
		FQuat BoneWorldRotation = PlayerCardMesh->GetBoneQuaternion(BoneName);

		NameWidgetComponent->SetWorldLocation(BoneWorldLocation + NameWidgetPos);
		NameWidgetComponent->SetWorldRotation(FRotator(BoneWorldRotation.X, BoneWorldRotation.Y, BoneWorldRotation.Z)- NameWidgetRot);
	}
	
	GlowPower = FMath::Clamp(GlowPower += DeltaTime* GlowPowerSpeed * GlowPowerDir, 0.f,2000.f);
	//UE_LOG(LogTemp, Warning, TEXT("GlowPower = %f"), GlowPower);
	Opacity = FMath::Clamp(Opacity += DeltaTime* 0.75f, 0.f, 1.f);
	if (PlayerCardDynamicMaterial)
	{
		PlayerCardDynamicMaterial->SetScalarParameterValue("GlowPower", GlowPower);
		PlayerCardDynamicMaterial->SetScalarParameterValue("Opacity", Opacity);
	}
}

void APlayerCard::PlayerCardEffect()
{

}

