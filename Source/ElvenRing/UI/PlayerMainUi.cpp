// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMainUi.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "ElvenRing/Character/UnitBase.h"
#include "UIManager.h"
//#include "ScreenEffectWidget.h"
//#include "ElvenRing/Core/ElvenringGameInstance.h"

void UPlayerMainUi::NativeConstruct()
{
	Super::NativeConstruct();
	//UElvenringGameInstance* ElvenringGameInstance = Cast< UElvenringGameInstance>(GetGameInstance());
	//ElvenringGameInstance->GetUIManager()->GetScreenEffectWidget()->FadeOutIn();
}
void UPlayerMainUi::SetActiveWidget(bool bShow)
{
	if (bShow)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerMainUi::DecreaseHp(float TargetHp, float HpMax)
{
	//HpProgressBar->SetPercent(1.f - (CurHp / HpMax));
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	HpProgressBar->SetPercent(TargetHp / HpMax);
	FRamdaElement FElement;
	FElement.TargetValue = TargetHp;
	FElement.ValueMax = HpMax;
	FElement.Duration = 0.5f;
	FElement.DelayTime = 1.f;
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

	MpProgressBar->SetPercent(TargetMp / MpMax);

	FRamdaElement FElement;
	FElement.TargetValue = TargetMp;
	FElement.ValueMax = MpMax;
	FElement.Duration = 0.5f;
	FElement.DelayTime = 1.f;
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

	float TargetPercent = TargetSt / StMax;
	StProgressBar->SetPercent(TargetPercent);
	StProgressYellowBar->SetPercent(TargetPercent);
}

void UPlayerMainUi::RecoverHp(float TargetHp, float HpMax,float RecoverSpeed )
{
	GetWorld()->GetTimerManager().ClearTimer(HpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(HpTimerDelayHandle);

	FRamdaElement FElement;
	FElement.TargetValue = TargetHp;
	FElement.ValueMax = HpMax;
	FElement.Duration = 0.5f;
	FElement.DelayTime = 1.f;
	FElement.MyProgressBar = HpProgressBar;
	FElement.MyYellowProgressBar = HpProgressYellowBar;
	FElement.DelayTimerHandle = &HpTimerDelayHandle;
	FElement.TimerHandle = &HpTimerHandle;
	RecoverProgressBar1(FElement);
}

void UPlayerMainUi::RecoverMp(float TargetMp, float MpMax, float RecoverSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(MpTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(MpTimerDelayHandle);

	FRamdaElement FElement;
	FElement.TargetValue = TargetMp;
	FElement.ValueMax = MpMax;
	FElement.Duration = 0.5f;
	FElement.DelayTime = 1.f;
	FElement.MyProgressBar = MpProgressBar;
	FElement.MyYellowProgressBar = MpProgressYellowBar;
	FElement.DelayTimerHandle = &MpTimerDelayHandle;
	FElement.TimerHandle = &MpTimerHandle;
	RecoverProgressBar1(FElement);
}

void UPlayerMainUi::RecoverSt(float TargetSt, float StMax, float RecoverSpeed)
{
	GetWorld()->GetTimerManager().ClearTimer(StTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StTimerDelayHandle);

	float TargetPercent = TargetSt / StMax;
	StProgressBar->SetPercent(TargetPercent);
	StProgressYellowBar->SetPercent(TargetPercent);
}

void UPlayerMainUi::LevelUp(float Hp, float St, float Mp)
{
	UpdateFrameSize(HpProgressBar,HpProgressYellowBar,HpFrameImg2, HpFrameImg3, Hp);
	UpdateFrameSize(StProgressBar,StProgressYellowBar,StFrameImg2, StFrameImg3, St);
	UpdateFrameSize(MpProgressBar,MpProgressYellowBar,MpFrameImg2, MpFrameImg3, Mp);
}

void UPlayerMainUi::UpdateHp(float CurHp, float HpMax, int32 State)
{
	if(State == 0)
		DecreaseHp(CurHp, HpMax);
	else
		RecoverHp(CurHp, HpMax);
}
void UPlayerMainUi::UpdateMp(float CurMp, float MpMax, int32 State)
{
	if (State == 0)
		DecreaseMp(CurMp, MpMax);
	else
		RecoverMp(CurMp, MpMax);
}
void UPlayerMainUi::UpdateSt(float CurSt, float StMax, int32 State)
{
	if (State == 0)
		DecreaseSt(CurSt, StMax);
	else
		RecoverSt(CurSt, StMax);
}

void UPlayerMainUi::BindToPlayer(AUnitBase* Player)
{
	if (Player)
	{
		Player->OnHpChanged.AddDynamic(this, &UPlayerMainUi::UpdateHp);
		Player->OnStaminaChanged.AddDynamic(this, &UPlayerMainUi::UpdateSt);
		Player->OnMpChanged.AddDynamic(this, &UPlayerMainUi::UpdateMp);

		AddToViewport(); //테스트버젼 임시로 해놓은거임. 0406 ksw
	}
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
	if (GetWorld())
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void UPlayerMainUi::DecreaseProgressBar(FRamdaElement& FEmt)
{
	FEmt.CurProgressBarPer = FEmt.MyYellowProgressBar->GetPercent();
	FEmt.TargetProgressBarPer = FEmt.TargetValue / FEmt.ValueMax;
	TWeakObjectPtr<UPlayerMainUi> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		
		*FEmt.DelayTimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, FEmt]() mutable
		{
			if (!SafeThis.IsValid())return;

			UWorld* World = SafeThis->GetWorld();

			if (!World)return;
			FEmt.PrevTime = World->GetTimeSeconds();
			World->GetTimerManager().SetTimer
			(
				*FEmt.TimerHandle,
				FTimerDelegate::CreateLambda([SafeThis, FEmt]() mutable
				{
					if (!SafeThis.IsValid())return;

					UWorld* World = SafeThis->GetWorld();

					if (!World)return;

					FEmt.ElapsedTime += World->GetTimeSeconds() - FEmt.PrevTime;
					float Alpha = FMath::Clamp(FEmt.ElapsedTime / FEmt.Duration, 0.f, 1.f);
					float CurValue = FMath::Lerp(FEmt.CurProgressBarPer, FEmt.TargetProgressBarPer, Alpha);
					//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());
					FEmt.MyYellowProgressBar->SetPercent(CurValue);

					if (1.f <= Alpha)
					{
						FEmt.MyYellowProgressBar->SetPercent(FEmt.TargetProgressBarPer);
						FEmt.ClearPointer();
						World->GetTimerManager().ClearTimer(*FEmt.TimerHandle);
					}
					else 
						FEmt.PrevTime = World->GetTimeSeconds();

				}),0.05f,true
			);
			//==========================================================================	
		}), FEmt.DelayTime,false
	);
	//FTimerDelegate TimerDel;
	//TimerDel.BindUFunction(this, FName("HpYellowBar"), CurYellowBarHpPer, TargetHpPer);
	//GetWorld()->GetTimerManager().SetTimer(HpTimerHandle, TimerDel, 0.05f, true);

	//GetWorld()->GetTimerManager().SetTimer(HpTimerDelayHandle, this, &UPlayerMainUi::HpTimerDelayDelay, 2.f, false);
}
void UPlayerMainUi::RecoverProgressBar1(FRamdaElement& FEmt)
{
	FEmt.CurProgressBarPer = FEmt.MyProgressBar->GetPercent();
	FEmt.TargetProgressBarPer = FEmt.TargetValue / FEmt.ValueMax;
	TWeakObjectPtr<UPlayerMainUi> SafeThis = this;
	GetWorld()->GetTimerManager().SetTimer
	(
		*FEmt.DelayTimerHandle,
		FTimerDelegate::CreateLambda([SafeThis, FEmt]() mutable
			{
				if (!SafeThis.IsValid() )return;
				UWorld* World = SafeThis->GetWorld();
				if (!World)return;

				FEmt.PrevTime = World->GetTimeSeconds();
				World->GetTimerManager().SetTimer
				(
					*FEmt.TimerHandle,
					FTimerDelegate::CreateLambda([SafeThis, FEmt]() mutable
						{
							if (!SafeThis.IsValid() )return;

							UWorld* World = SafeThis->GetWorld();

							if (!World)return;

							FEmt.ElapsedTime += World->GetTimeSeconds() - FEmt.PrevTime;
							float Alpha = FMath::Clamp(FEmt.ElapsedTime / FEmt.Duration, 0.f, 1.f);
							float CurValue = FMath::Lerp(FEmt.CurProgressBarPer, FEmt.TargetProgressBarPer, Alpha);
							//UE_LOG(LogTemp, Warning, TEXT("ElapsedTime : %f / PrevTime :  %f / TimeSeconds :  %f "), FEmt.ElapsedTime, FEmt.PrevTime, GetWorld()->GetTimeSeconds());
							FEmt.MyProgressBar->SetPercent(CurValue);

							if (1.f <= Alpha)
							{
								FEmt.MyProgressBar->SetPercent(FEmt.TargetProgressBarPer);
								FEmt.MyYellowProgressBar->SetPercent(FEmt.TargetProgressBarPer);
								FEmt.ClearPointer();
								World->GetTimerManager().ClearTimer(*FEmt.TimerHandle);
							}
							else
								FEmt.PrevTime = World->GetTimeSeconds();
						}), 0.05f, true
				);
			}), FEmt.DelayTime, false
	);
}

