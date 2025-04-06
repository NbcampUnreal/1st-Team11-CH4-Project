// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMainUi.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UPlayerMainUi::NativeConstruct()
{
	Super::NativeConstruct();

	//PortraitImages.Add(HpFrameImg2);
	//PortraitImages.Add(HpFrameImg3);

	//Character->OnHealthChanged.AddDynamic(this,&UPlayerMainUi::DecreaseMp);
	//Character->OnHealthChanged.AddDynamic(this,&UPlayerMainUi::DecreaseMp);
	//Character->OnHealthChanged.AddDynamic(this,&UPlayerMainUi::DecreaseSt);
	//Character->OnHealthChanged.AddDynamic(this,&UPlayerMainUi::LevelUp);
}

void UPlayerMainUi::DecreaseHp(float TargetHp, float HpMax)
{
	//HpProgressBar->SetPercent(1.f - (CurHp / HpMax));
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FRamdaElement FElement;
	FElement.TargetValue = TargetHp;
	FElement.ValueMax = HpMax;
	FElement.Duration = 1.0f;
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyYellowProgressBar = HpProgressYellowBar;
	FElement.DelayTimerHandle = &HpTimerDelayHandle;
	FElement.TimerHandle = &HpTimerHandle;

	DecreaseProgressBar(FElement);
}
void UPlayerMainUi::DecreaseMp(float TargetMp, float MpMax)
{
	//MpProgressBar->SetPercent(1.f - (TargetMp / MpMax));
	GetWorld()->GetTimerManager().ClearTimer(MpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MpTimerDelayHandle);

	FRamdaElement FElement;
	FElement.TargetValue = TargetMp;
	FElement.ValueMax = MpMax;
	FElement.Duration = 1.0f;
	FElement.MyProgressBar = MpProgressBar;
	FElement.MyYellowProgressBar = MpProgressYellowBar;
	FElement.DelayTimerHandle = &MpTimerDelayHandle;
	FElement.TimerHandle = &MpTimerHandle;

	DecreaseProgressBar(FElement);
}
void UPlayerMainUi::DecreaseSt(float TargetSt, float StMax)
{
	//StProgressBar->SetPercent(1.f - (TargetSt / StMax));
	
	GetWorld()->GetTimerManager().ClearTimer(StTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StTimerDelayHandle);

	FRamdaElement FElement;
	FElement.TargetValue = TargetSt;
	FElement.ValueMax = StMax;
	FElement.Duration = 1.0f;
	FElement.MyProgressBar = StProgressBar;
	FElement.MyYellowProgressBar = StProgressYellowBar;
	FElement.DelayTimerHandle = &StTimerDelayHandle;
	FElement.TimerHandle = &StTimerHandle;

	DecreaseProgressBar(FElement);
}

void UPlayerMainUi::RecoverHp(float TargetHp, float HpMax,float RecoverSpeed )
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FRamdaElement2 FEmt;
	FEmt.TargetValue = TargetHp;
	FEmt.ValueMax = HpMax;
	FEmt.Duration = RecoverSpeed;
	FEmt.TimerHandle = &HpTimerHandle;
	FEmt.DelayTimerHandle = &HpTimerDelayHandle;
	FEmt.MyProgressBar = HpProgressBar;
	FEmt.TargetYellowProgressBar = HpProgressYellowBar;

	RecoverProgressBar(FEmt);
}

void UPlayerMainUi::RecoverMp(float TargetMp, float MpMax, float RecoverSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(MpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MpTimerDelayHandle);

	FRamdaElement2 FEmt;

	FEmt.TargetValue = TargetMp;
	FEmt.ValueMax = MpMax;
	FEmt.Duration = RecoverSpeed;
	FEmt.TimerHandle = &MpTimerHandle;
	FEmt.DelayTimerHandle = &MpTimerDelayHandle;
	FEmt.MyProgressBar = MpProgressBar;
	FEmt.TargetYellowProgressBar = MpProgressYellowBar;

	RecoverProgressBar(FEmt);
}

void UPlayerMainUi::RecoverSt(float TargetSt, float StMax, float RecoverSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(StTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StTimerDelayHandle);

	FRamdaElement2 FEmt;

	FEmt.TargetValue = TargetSt;
	FEmt.ValueMax = StMax;
	FEmt.Duration = RecoverSpeed;
	FEmt.TimerHandle = &StTimerHandle;
	FEmt.DelayTimerHandle = &StTimerDelayHandle;
	FEmt.MyProgressBar = StProgressBar;
	FEmt.TargetYellowProgressBar = StProgressYellowBar;

	RecoverProgressBar(FEmt);
}

void UPlayerMainUi::LevelUp(float Hp, float St, float Mp)
{
	UpdateFrameSize(HpProgressBar,HpProgressYellowBar,HpFrameImg2, HpFrameImg3, Hp);
	UpdateFrameSize(StProgressBar,StProgressYellowBar,StFrameImg2, StFrameImg3, St);
	UpdateFrameSize(MpProgressBar,MpProgressYellowBar,MpFrameImg2, MpFrameImg3, Mp);
}

void UPlayerMainUi::UpdateFrameSize(UProgressBar* ProgressBar, UProgressBar* ProgressBar2, UImage* FrameImage2, UImage* FrameImage3, float Value )
{
	float XSize = 0.f;
	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();
		XSize = CurrentSize.X * Value;
		ProgressBarCanvasSlot->SetSize(FVector2D(XSize, CurrentSize.Y));

		if (UCanvasPanelSlot* ProgressBarCanvasSlot2 = Cast<UCanvasPanelSlot>(ProgressBar2->Slot))
			ProgressBarCanvasSlot2->SetSize(FVector2D(XSize, CurrentSize.Y));

		if (UCanvasPanelSlot* FrameCanvasSlot2 = Cast<UCanvasPanelSlot>(FrameImage2->Slot))
		{
			CurrentSize = FrameCanvasSlot2->GetSize();
			FrameCanvasSlot2->SetSize(FVector2D(XSize, CurrentSize.Y));
		}

		if (UCanvasPanelSlot* FrameCanvasSlot3 = Cast<UCanvasPanelSlot>(FrameImage3->Slot))
		{
			//UE_LOG(LogTemp, Warning, TEXT("%f"), ProgressBarCanvasSlot->GetPosition().X + XSize);
			FrameCanvasSlot3->SetPosition(FVector2D(ProgressBarCanvasSlot->GetPosition().X + XSize, FrameCanvasSlot3->GetPosition().Y));
		}
	}
}

void UPlayerMainUi::UpdateProgressBarSize(UProgressBar* ProgressBar,float XSize )
{
	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar->Slot))
	{
		FVector2D CurrentSize = ProgressBarCanvasSlot->GetSize();
		ProgressBarCanvasSlot->SetSize( FVector2D( XSize, CurrentSize.Y ) );
	}
}

void UPlayerMainUi::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerMainUi::DecreaseProgressBar(FRamdaElement& FEmt)
{
	
	FVector2D CurrentSize = FVector2D::Zero();
	if (UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(FEmt.MyYellowProgressBar->Slot))
		CurrentSize = ProgressBarCanvasSlot->GetSize();
	float TargetPercent = FEmt.TargetValue / FEmt.ValueMax;
	float UpdateProgressBarSizeX = CurrentSize.X * TargetPercent;

	UpdateProgressBarSize(FEmt.MyProgressBar, UpdateProgressBarSizeX);

	//if (true)return;
	float CurYellowBarHpPer = FEmt.MyYellowProgressBar->GetPercent();
	float TargetHpPer = 1.f - TargetPercent;

	FEmt.CurProgressBarPer = CurYellowBarHpPer;
	FEmt.TargetProgressBarPer = TargetHpPer;

	GetWorld()->GetTimerManager().SetTimer
	(
		*FEmt.DelayTimerHandle,
		FTimerDelegate::CreateLambda([this, FEmt]() mutable
		{
			FEmt.PrevTime = GetWorld()->GetTimeSeconds();
			//==========================================================================
			GetWorld()->GetTimerManager().SetTimer
			(
				*FEmt.TimerHandle,
				FTimerDelegate::CreateLambda([this, FEmt]() mutable
				{
					FEmt.ElapsedTime += GetWorld()->GetTimeSeconds() - FEmt.PrevTime;
					float Alpha = FMath::Clamp(FEmt.ElapsedTime / FEmt.Duration, 0.f, 1.f);
					float CurValue = FMath::Lerp(FEmt.CurProgressBarPer, FEmt.TargetProgressBarPer, Alpha);
					//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());
					FEmt.MyYellowProgressBar->SetPercent(CurValue);

					if (1.f <= Alpha)
					{
						FEmt.MyYellowProgressBar->SetPercent(FEmt.TargetProgressBarPer);
						FEmt.ClearPointer();
						GetWorld()->GetTimerManager().ClearTimer(*FEmt.TimerHandle);
					}
					FEmt.PrevTime = GetWorld()->GetTimeSeconds();
				}),0.05f,true
			);
			//==========================================================================	
		}),1.5f,false
	);
	//FTimerDelegate TimerDel;
	//TimerDel.BindUFunction(this, FName("HpYellowBar"), CurYellowBarHpPer, TargetHpPer);
	//GetWorld()->GetTimerManager().SetTimer(HpTimerHandle, TimerDel, 0.05f, true);

	//GetWorld()->GetTimerManager().SetTimer(HpTimerDelayHandle, this, &UPlayerMainUi::HpTimerDelayDelay, 2.f, false);
}

void UPlayerMainUi::RecoverProgressBar(FRamdaElement2& FEmt)
{
	FVector2D StandardSize = FVector2D::Zero();
	FVector2D CurrentSize = FVector2D::Zero();
	UCanvasPanelSlot* TargetProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(FEmt.TargetYellowProgressBar->Slot);
	if (TargetProgressBarCanvasSlot)
		StandardSize = TargetProgressBarCanvasSlot->GetSize();
	else
		return;
	UCanvasPanelSlot* MyProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(FEmt.MyProgressBar->Slot);
	if (MyProgressBarCanvasSlot)
		CurrentSize = MyProgressBarCanvasSlot->GetSize();
	else
		return;
	float RecoverPercent = FEmt.TargetValue / FEmt.ValueMax;
	float UpdateProgressBarSizeX = StandardSize.X * RecoverPercent;
	FVector2D TargetSize = FVector2D(UpdateProgressBarSizeX, CurrentSize.Y);

	FEmt.CurrentSize = CurrentSize;
	FEmt.TargetSize = TargetSize;
	FEmt.RecoverProgessBarPercent = RecoverPercent;

	//FEmt.PrevTime = GetWorld()->GetTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer
	(
		*FEmt.DelayTimerHandle,
		FTimerDelegate::CreateLambda([this, FEmt, MyProgressBarCanvasSlot]() mutable
		{
			//UE_LOG(LogTemp, Warning, TEXT("CurrentSize : %f /TargetSize :  %f "), FEmt.CurrentSize.X, FEmt.TargetSize.X);
			FEmt.PrevTime = GetWorld()->GetTimeSeconds();
			GetWorld()->GetTimerManager().SetTimer
			(
				*FEmt.TimerHandle,
				FTimerDelegate::CreateLambda([this, FEmt, MyProgressBarCanvasSlot]() mutable
				{
					FEmt.ElapsedTime += GetWorld()->GetTimeSeconds() - FEmt.PrevTime;
					float Alpha = FMath::Clamp(FEmt.ElapsedTime / FEmt.Duration, 0.f, 1.f);
					FVector2D RenewalSize = FMath::Lerp(FEmt.CurrentSize, FEmt.TargetSize, Alpha);
					//UE_LOG(LogTemp, Warning, TEXT("CurrentSize : %f / RenewalSize : %f / TargetSize :  %f "), FEmt.CurrentSize.X, RenewalSize.X, FEmt.TargetSize.X);
					MyProgressBarCanvasSlot->SetSize(RenewalSize);

					if (1.f <= Alpha)
					{
						MyProgressBarCanvasSlot->SetSize(FEmt.TargetSize);
						FEmt.TargetYellowProgressBar->SetPercent(FEmt.RecoverProgessBarPercent);
						FEmt.ClearPointer();
						GetWorld()->GetTimerManager().ClearTimer(*FEmt.TimerHandle);
					}
					FEmt.PrevTime = GetWorld()->GetTimeSeconds();
				}), 0.05f, true
			);
		}), 1.5f, false
	);
}

