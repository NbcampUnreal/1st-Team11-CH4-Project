// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMainUi.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UPlayerMainUi::NativeConstruct()
{
	Super::NativeConstruct();

	PortraitImages.Add(HpFrameImg2);
	PortraitImages.Add(HpFrameImg3);

	//Character->OnHpChaged.AddDynamic(this,&UPlayerMainUi::);
	
}
void UPlayerMainUi::UpdateHpBar(float Hp)
{
	HpProgressBar->SetPercent(1.f - (Hp / 100.f));
}
void UPlayerMainUi::UpdateMpBar(float Mp)
{
	//HealthBar->SetPercent(1.f - (Hp / 100.f));
}

void UPlayerMainUi::LevelUp(float Hp, float St, float Mp)
{
	FixFrameSize(HpProgressBar, HpFrameImg2, HpFrameImg3, Hp);
	FixFrameSize(StProgressBar, StFrameImg2, StFrameImg3, St);
	FixFrameSize(MpProgressBar, MpFrameImg2, MpFrameImg3, Mp);
}

void UPlayerMainUi::FixFrameSize(UProgressBar* ProgressBar, UImage* FrameImage2, UImage* FrameImage3, float Value )
{
	float XSize = 0.f;
	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();
		XSize = CurrentSize.X * Value;
		ProgressBarCanvasSlot->SetSize(FVector2D(XSize, CurrentSize.Y));

		if (UCanvasPanelSlot* FrameCanvasSlot2 = Cast<UCanvasPanelSlot>(FrameImage2->Slot))
		{
			CurrentSize = FrameCanvasSlot2->GetSize();
			FrameCanvasSlot2->SetSize(FVector2D(XSize, CurrentSize.Y));
		}

		if (UCanvasPanelSlot* FrameCanvasSlot3 = Cast<UCanvasPanelSlot>(FrameImage3->Slot))
		{
			UE_LOG(LogTemp, Warning, TEXT("%f"), ProgressBarCanvasSlot->GetPosition().X + XSize);
			FrameCanvasSlot3->SetPosition(FVector2D(ProgressBarCanvasSlot->GetPosition().X + XSize, FrameCanvasSlot3->GetPosition().Y));
		}
	}
}

void UPlayerMainUi::NativeDestruct()
{
	Super::NativeDestruct();
}
