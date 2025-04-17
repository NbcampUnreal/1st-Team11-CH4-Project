
#include "ElvenRingTitleGameMode.h"
#include "ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/UI/TitleMapParticle.h"

AElvenRingTitleGameMode::AElvenRingTitleGameMode()
{
	DefaultPawnClass = nullptr;
	
	//static ConstructorHelpers::FClassFinder<ATitleMapParticle> BP_TitleMapParticle(TEXT("/Game/ElvenRing/Blueprints/UI/BP_TtitleParticle"));
	//if (BP_TitleMapParticle.Succeeded())
	//	TitleMapParticleClass = BP_TitleMapParticle.Class;
}

void AElvenRingTitleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AElvenRingTitleGameMode::BeginPlay()
{
	Super::BeginPlay();
	//UElvenringGameInstance* EGameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	//if (EGameInstance)
	//{
	//	EGameInstance->GetUIManager()->ShowTitleScreen(GetWorld());
	//}

	//FActorSpawnParameters SpawnParams;
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//FVector Location(0, 0, 100);
	//FRotator Rotation = FRotator::ZeroRotator;

	//ATitleMapParticle* TitleParticle = GetWorld()->SpawnActor<ATitleMapParticle>(TitleMapParticleClass, Location, Rotation, SpawnParams);
	//int32 dsfsdf = 0;
}
