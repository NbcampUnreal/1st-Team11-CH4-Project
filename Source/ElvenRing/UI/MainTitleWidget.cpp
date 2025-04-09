// Fill out your copyright notice in the Description page of Project Settings.


#include "MainTitleWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UMainTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainTitleWidget::OnClicked);
        StartButton->OnPressed.AddDynamic(this, &UMainTitleWidget::OnPressed);
        StartButton->OnHovered.AddDynamic(this, &UMainTitleWidget::OnHovered);
        StartButton->OnUnhovered.AddDynamic(this, &UMainTitleWidget::OnUnhovered);
        StartButton->OnReleased.AddDynamic(this, &UMainTitleWidget::OnReleased);

        Dir = 1.f;
        GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);
	}
}

void UMainTitleWidget::NativeDestruct()
{
    Super::NativeDestruct();
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
}

void UMainTitleWidget::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Click StartBTN"));
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);

    UGameplayStatics::OpenLevel(this, FName("WaitingRoomMap"));
}
void UMainTitleWidget::OnPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("OnPressed"));
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    FLinearColor Color = TextBlock->ColorAndOpacity.GetSpecifiedColor();
    Color.A = 1.f;
    TextBlock->SetColorAndOpacity(Color);

  //  GetWorld()->GetTimerManager().ClearTimer(OnHoverTimerHandle);
  //  GetWorld()->GetTimerManager().SetTimer(OnPressTimerHandle, this, &UMainTitleWidget::OnPressed, 0.05f, true);*/
}
void UMainTitleWidget::OnHovered()
{
    UE_LOG(LogTemp, Warning, TEXT("OnHovered"));
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    FLinearColor Color = TextBlock->ColorAndOpacity.GetSpecifiedColor();
    Color.A = 1.f;
    TextBlock->SetColorAndOpacity(Color);

    //GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    //GetWorld()->GetTimerManager().SetTimer(OnHoverTimerHandle, this, &UMainTitleWidget::HoverText, 0.05f, true);
}
void UMainTitleWidget::OnUnhovered()
{
    UE_LOG(LogTemp, Warning, TEXT("OnUnhovered"));
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);
    //GetWorld()->GetTimerManager().ClearTimer(OnHoverTimerHandle);
    //GetWorld()->GetTimerManager().SetTimer(OnUnHoverTimerHandle, this, &UMainTitleWidget::UnhoverText, 0.05f, true);
}
void UMainTitleWidget::OnReleased()
{
    UE_LOG(LogTemp, Warning, TEXT("OnReleaseed"));
    GetWorld()->GetTimerManager().ClearTimer(AlphaPingpongTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(AlphaPingpongTimerHandle, this, &UMainTitleWidget::PingpongText, 0.05f, true);
}
void UMainTitleWidget::PingpongText()
{
    if (!TextBlock) return;

    FLinearColor Color = TextBlock->ColorAndOpacity.GetSpecifiedColor();

    if (Color.A <= 0.0f)
    {
        Dir = 1.0f;
    }
    else if (Color.A >= 1.0f)
    {
        Dir = -1.0f;
    }

    float CurTime = GetWorld()->GetTimeSeconds();
    float Elapsed = (CurTime - time) * Dir;

    if (Dir > 0)
        Color.A += FMath::Clamp(Elapsed, 0.f, 1.f);
    else
        Color.A += FMath::Clamp(Elapsed, -1.f, 0.f);

    Color.A = FMath::Clamp(Color.A, 0.f, 1.f);

    TextBlock->SetColorAndOpacity(Color);
    time = CurTime;

  //  UE_LOG(LogTemp, Warning, TEXT("Cur: %.2f | Dir: %.1f | Alpha: %.2f"), CurTime, Dir, Color.A);
}

void UMainTitleWidget::HoverText()
{
   // return;
    FWidgetTransform Transform = StartButton->RenderTransform;
    float CurTime = 0.f;
    if (1.2f > Transform.Scale.X)
    {
        CurTime = GetWorld()->GetTimeSeconds();
        float Elapsed = (CurTime - time) * Dir;

        Transform.Scale.X += Elapsed;
        Transform.Scale.Y += Elapsed;
        StartButton->SetRenderTransform(Transform);
    }
    else
    {
        Transform.Scale.X = 1.2f;
        Transform.Scale.Y = 1.2f;
        StartButton->SetRenderTransform(Transform);
    }
    time = CurTime;
}

void UMainTitleWidget::UnhoverText()
{
   // return;

    FWidgetTransform Transform = StartButton->RenderTransform;
    float CurTime = 0.f;
    if (1.0f < Transform.Scale.X)
    {
        CurTime = GetWorld()->GetTimeSeconds();
        float Elapsed = (CurTime - time) * Dir;

        Transform.Scale.X -= Elapsed;
        Transform.Scale.Y -= Elapsed;
        StartButton->SetRenderTransform(Transform);
    }
    else
    {
        Transform.Scale.X = 1.f;
        Transform.Scale.Y = 1.f;
        StartButton->SetRenderTransform(Transform);
    }
    time = CurTime;
}

void UMainTitleWidget::OnReleaseText()
{
}
