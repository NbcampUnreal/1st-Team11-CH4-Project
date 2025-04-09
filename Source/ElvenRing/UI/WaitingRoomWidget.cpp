
#include "WaitingRoomWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"


void UWaitingRoomWidget::OnStartButtonClicked()
{
    FString InputName = NicknameTextBox ? NicknameTextBox->GetText().ToString() : TEXT("Player");

    if (UElvenringGameInstance* ElvenringGameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
    {
        ElvenringGameInstance->SetNickname(InputName);

    }
    UGameplayStatics::OpenLevel(this, FName("ElvenRuins_fix"));
   // UE_LOG(LogTemp, Warning, TEXT("OpenLevel(this, TEXT(BattleMap))"));
}

void UWaitingRoomWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (OkButton)
    {
       OkButton->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnStartButtonClicked);
       //Dir = 1.f;
       //GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UWaitingRoomWidget::PingpongText, 0.05f, true);
    }
}

void UWaitingRoomWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
