
#include "WaitingRoomWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/WaitingRoomPlayerCardsRT.h"

void UWaitingRoomWidget::OnStartButtonClicked()
{
    // UGameplayStatics::OpenLevel(this, FName("ElvenRuins_fix"));
    GetWorld()->ServerTravel(TEXT("ElvenRuins_fix"));
   // UE_LOG(LogTemp, Warning, TEXT("OpenLevel(this, TEXT(BattleMap))"));
}

void UWaitingRoomWidget::OnOkNickNameButtonClicked()
{
    FString InputName = NicknameTextBox ? NicknameTextBox->GetText().ToString() : TEXT("Player");

    if (UElvenringGameInstance* ElvenringGameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
    {
        ElvenringGameInstance->SetNickname(InputName);
        TextPlayerCardNickName->SetText(FText::FromString(InputName));
        WaitingRoomPlayerCardsRT->SetName(FText::FromString(InputName),0);
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

    ShockWaves.Add(Image_ShockWave1);
    ShockWaves.Add(Image_ShockWave2);
    ShockWaves.Add(Image_ShockWave3);
    for (int32 i = 0; i < 3; ++i)
        ShockWaves[i]->SetVisibility(ESlateVisibility::Collapsed);

    OpenNamePopup();
    ShockTimerHandles.AddDefaulted(3);
}
void UWaitingRoomWidget::GuestMode()
{
    ImageReady->SetVisibility(ESlateVisibility::Visible);
    GameStartButton->SetVisibility(ESlateVisibility::Collapsed);
}
void UWaitingRoomWidget::HostMode()
{
    GameStartButton->SetVisibility(ESlateVisibility::Visible);
    ImageReady->SetVisibility(ESlateVisibility::Collapsed);
}
void UWaitingRoomWidget::SetPlayerCardRT(AWaitingRoomPlayerCardsRT* PlayerCardsRT)
{
    this->WaitingRoomPlayerCardsRT = PlayerCardsRT;
}
void UWaitingRoomWidget::PlayShockWave(int32 Index)
{
    if (bDegCntArr[Index] )
        return;
    bDegCntArr[Index] = true;

    //ShockWaves[Index]->SetVisibility(ESlateVisibility::Visible);
   // UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/ElvenRing/UI/M_ShockWave_Red_Inst.M_ShockWave_Red_Inst"));
   // UE_LOG(LogTemp, Warning, TEXT("ShockWaves cnt : %d"), cnt);
    FSlateBrush Brush = ShockWaves[Index]->Brush;
   UMaterialInterface* Mat = Cast<UMaterialInterface>(Brush.GetResourceObject());

    if (Mat && !Mat->IsA<UMaterialInstanceDynamic>())
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
        if (DynMat)
        {
            DynMat->SetScalarParameterValue("Speed",30 );//("StartFrame", GetWorld()->GetTimeSeconds() - 0.1f); // 0보다 조금만 과거
            Brush.SetResourceObject(DynMat);
            ShockWaves[Index]->SetBrush(Brush);
            ShockWaves[Index]->SetVisibility(ESlateVisibility::Visible);
            CloseShockWave(Brush, DynMat,Index);
        }
    }
   //UE_LOG(LogTemp, Warning, TEXT(" GetWorld()->GetTimeSeconds() %f"), GetWorld()->GetTimeSeconds());
}
void UWaitingRoomWidget::OpenNamePopup()
{
    FTimerHandle TimerHandle;
   GetWorld()->GetTimerManager().SetTimer
    (
        TimerHandle,
        FTimerDelegate::CreateLambda([this]() mutable
        {
            PlayAnimation(NickNameFrameAppeare);
           // UE_LOG(LogTemp, Warning, TEXT("Clear!!!!!! TimerHandle/ Index : %d"), Index);
        }), 0.1f, false
    );//
}
void UWaitingRoomWidget::CloseShockWave(FSlateBrush& Brush, UMaterialInstanceDynamic* DynMat,int Index)
{
  //  UMaterialInterface* Mat = Cast<UMaterialInterface>(Brush.GetResourceObject());
   // UMaterialInterface* Mat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/ElvenRing/UI/M_ShockWave_Red_Inst.M_ShockWave_Red_Inst"));
  //  FSlateBrush Brush1 = ShockWaves[Index]->Brush;

  //  if (Mat && !Mat->IsA<UMaterialInstanceDynamic>())
    {
    //    UMaterialInstanceDynamic* DynMat1 = UMaterialInstanceDynamic::Create(Mat, this);
      //  if (DynMat1)
        float ElapsedTime = 0.f;
       // FTimerHandle TimerHandle2 ;
        {
            GetWorld()->GetTimerManager().SetTimer(
                ShockTimerHandles[Index],
                FTimerDelegate::CreateLambda([this, ElapsedTime,  &Brush, Index, DynMat]() mutable
                    {
                        ElapsedTime += 0.01f;
                        DynMat->SetScalarParameterValue("ElapsedTime", ElapsedTime);
                   
                        //UE_LOG(LogTemp, Warning, TEXT("ElapsedTime %f / idx %d"), ElapsedTime, Index);
                        if (ElapsedTime >= 2.f)
                        {
                            DynMat->SetScalarParameterValue("Speed", 0.0f);
                            ShockWaves[Index]->SetVisibility(ESlateVisibility::Collapsed);
                            FSlateBrush Empty;
                            ShockWaves[Index]->SetBrush(Empty);
                            bDegCntArr[Index] = false;

                           // UE_LOG(LogTemp, Warning, TEXT("Shockwave 종료 (Index %d)"), Index);
                            GetWorld()->GetTimerManager().ClearTimer(ShockTimerHandles[Index]);
                        }
                    }),
                0.01f, true
            );
        }
    }
}
void UWaitingRoomWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
