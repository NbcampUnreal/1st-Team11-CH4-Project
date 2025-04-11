// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTitleWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SinglePlayButton && MultiPlayButton)
	{
		SinglePlayButton->OnClicked.AddDynamic(this, &UMainTitleWidget::OnSingleButtonClicked);
        MultiPlayButton->OnClicked.AddDynamic(this, &UMainTitleWidget::OnMultiButtonClicked);
        Dir = 1.f;
       // GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);
	}
}

void UMainTitleWidget::NativeDestruct()
{
    Super::NativeDestruct();
    //GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

//void UMainTitleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
//{
//    Super::NativeTick(MyGeometry, InDeltaTime);
//}

void UMainTitleWidget::OnSingleButtonClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("Click StartBTN"));

   // GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
   // GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);
    UGameplayStatics::OpenLevel(this, FName("ElvenRuins_fix"));
}
void UMainTitleWidget::OnMultiButtonClicked()
{
    if (ConnecState != EConnectState::Ready)
        return;
    ConnecState = EConnectState::Connecting;

    SinglePlayButton->SetIsEnabled(false);
    MultiPlayButton->SetIsEnabled(false);

    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);

    //ConnetingTimer();
     UGameplayStatics::OpenLevel(this, FName("WaitingRoomMap"));
}

void UMainTitleWidget::PingpongText()
{
    if (!TextConnection) return;

    FLinearColor Color = TextConnection->GetColorAndOpacity().GetSpecifiedColor(); 

    if (Color.A <= 0.0f)
        Dir = 1.0f;
    else if (Color.A >= 1.0f)
        Dir = -1.0f;

    float CurTime = GetWorld()->GetTimeSeconds()*1.5f;
    float Elapsed = (CurTime - time) * Dir;

    if (Dir > 0)
        Color.A += FMath::Clamp(Elapsed, 0.f, 1.f);
    else
        Color.A += FMath::Clamp(Elapsed, -1.f, 0.f);

    Color.A = FMath::Clamp(Color.A, 0.f, 1.f);

    TextConnection->SetColorAndOpacity(Color);
    time = CurTime;
  //  UE_LOG(LogTemp, Warning, TEXT("Cur: %.2f | Dir: %.1f | Alpha: %.2f"), CurTime, Dir, Color.A);
}

void UMainTitleWidget::ConnetingTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
   
    FString ErrorMassege = TEXT("Connecting..");
    TextConnection->SetText(FText::FromString(ErrorMassege));

	FRamdaConnect FRConnect;
	FRConnect.TimerHandle = &TimerHandle;

    FRConnect.PrevTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		*FRConnect.TimerHandle,
		FTimerDelegate::CreateLambda([this, FRConnect]() mutable
		{
            FRConnect.ElapsedTime += GetWorld()->GetTimeSeconds() - FRConnect.PrevTime;
				
			if (10.f <= FRConnect.ElapsedTime)
			{
                GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);

                FString ErrorMassege = TEXT("Connection failed.Please try again");

                TextConnection->SetColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f));
                TextConnection->SetText(FText::FromString(ErrorMassege));

                CloseConnectMaseege(FRConnect);
				//GetWorld()->GetTimerManager().ClearTimer(*FRConnect.TimerHandle);
			}
            FRConnect.PrevTime = GetWorld()->GetTimeSeconds();
		}), 0.05f, true
	);
}
void UMainTitleWidget::CloseConnectMaseege(FRamdaConnect& FRConnect)
{
    GetWorld()->GetTimerManager().ClearTimer(*FRConnect.TimerHandle);

    FRConnect.TimerHandle = &TimerHandle;
    FRConnect.ElapsedTime = 0.f;

    FRConnect.PrevTime = GetWorld()->GetTimeSeconds();
    GetWorld()->GetTimerManager().SetTimer
    (
        *FRConnect.TimerHandle,
        FTimerDelegate::CreateLambda([this, FRConnect]() mutable
        {
            FRConnect.ElapsedTime += GetWorld()->GetTimeSeconds() - FRConnect.PrevTime;

            if (1.f <= FRConnect.ElapsedTime)
            {
                FString ErrorMassege = TEXT("");
                TextConnection->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
                TextConnection->SetText(FText::FromString(ErrorMassege));
                SinglePlayButton->SetIsEnabled(true);
                MultiPlayButton->SetIsEnabled(true);
                ConnecState = EConnectState::Ready;
                GetWorld()->GetTimerManager().ClearTimer(*FRConnect.TimerHandle);
            }
            FRConnect.PrevTime = GetWorld()->GetTimeSeconds();
        }), 0.05f, true
    );
}
