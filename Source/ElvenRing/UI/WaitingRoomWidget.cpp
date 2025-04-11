
#include "WaitingRoomWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"


void UWaitingRoomWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, FName("ElvenRuins_fix"));
   // UE_LOG(LogTemp, Warning, TEXT("OpenLevel(this, TEXT(BattleMap))"));
}

void UWaitingRoomWidget::OnOkNickNameButtonClicked()
{
    FString InputName = NicknameTextBox ? NicknameTextBox->GetText().ToString() : TEXT("Player");

    if (UElvenringGameInstance* ElvenringGameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
    {
        ElvenringGameInstance->SetNickname(InputName);
        TextPlayerCardNickName->SetText(FText::FromString(InputName));
        OverlayNickNameFrame->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UWaitingRoomWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OkNickNameButton->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnOkNickNameButtonClicked);
    GameStartButton->OnClicked.AddDynamic(this, &UWaitingRoomWidget::OnStartButtonClicked);
    //Dir = 1.f;
    //GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UWaitingRoomWidget::PingpongText, 0.05f, true);
}

void UWaitingRoomWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
