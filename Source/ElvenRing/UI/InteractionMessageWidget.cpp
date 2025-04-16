// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMessageWidget.h"

#include "Components/TextBlock.h"
#include "ElvenRing/Character/ElvenRingCharacter.h"

void UInteractionMessageWidget::ShowMessageText_Implementation(const FString& Message)
{
	if (bIsActive && GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);
	
	TextMessage->SetText(FText::FromString(Message));
	PlayFadeInAnimation();
}

void UInteractionMessageWidget::HideMessageText()
{
	PlayFadeOutAnimation();
}

void UInteractionMessageWidget::BindToPlayer(AUnitBase* Unit)
{
	if (AElvenRingCharacter* Player = Cast<AElvenRingCharacter>(Unit))
	{
		if (UInteractionComponent* InteractionComponent = Player->FindComponentByClass<UInteractionComponent>())
		{
			InteractionComponent->OnInteractableFound.AddDynamic(this, &UInteractionMessageWidget::ShowMessageText);
			InteractionComponent->OnInteractableLost.AddDynamic(this, &UInteractionMessageWidget::HideMessageText);
		}
		else
		{
			UE_LOG(LogTemp,Display, TEXT("InteractionComponent is not found"));
		}
	}
}

void UInteractionMessageWidget::SetActiveWidget(bool bActive)
{
	bIsActive = bActive;
	if (bActive)
	{
		if (bIsForwarded)
		{
			SetVisibility(ESlateVisibility::Visible);
			PlayFadeInAnimation();
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
		StopAnimation(FadeAnim);
	}
}

void UInteractionMessageWidget::OnFadeAnimationEnd()
{
	if (!bIsForwarded)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInteractionMessageWidget::PlayFadeInAnimation()
{
	if (FadeAnim)
	{
		bIsForwarded = true;
		OnFadeFinishedDelegate.Unbind();
		OnFadeFinishedDelegate.BindDynamic(this, &UInteractionMessageWidget::OnFadeAnimationEnd);
		
		UnbindAllFromAnimationFinished(FadeAnim);
		BindToAnimationFinished(FadeAnim, OnFadeFinishedDelegate);

		PlayAnimationForward(FadeAnim);
	}
}

void UInteractionMessageWidget::PlayFadeOutAnimation()
{
	if (FadeAnim)
	{
		bIsForwarded = false;
		OnFadeFinishedDelegate.Unbind();
		OnFadeFinishedDelegate.BindDynamic(this, &UInteractionMessageWidget::OnFadeAnimationEnd);
		
		UnbindAllFromAnimationFinished(FadeAnim);
		BindToAnimationFinished(FadeAnim, OnFadeFinishedDelegate);
		
		PlayAnimationReverse(FadeAnim);
	}
}

void UInteractionMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractionMessageWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
