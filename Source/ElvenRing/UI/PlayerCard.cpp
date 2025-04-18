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
void APlayerCard::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (NameWidgetComponent && PlayerCardMesh)
	{
		FVector BoneWorldLocation = PlayerCardMesh->GetBoneLocation(BoneName);
		FQuat BoneWorldRotation = PlayerCardMesh->GetBoneQuaternion(BoneName);

		NameWidgetComponent->SetWorldLocation(BoneWorldLocation + NameWidgetPos);
		NameWidgetComponent->SetWorldRotation(FRotator(BoneWorldRotation.X, BoneWorldRotation.Y, BoneWorldRotation.Z)- NameWidgetRot);
	}
}

void APlayerCard::PlayerCardEffect()
{

}

