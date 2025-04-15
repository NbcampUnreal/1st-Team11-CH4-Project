// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionMessageWidget.h"

#include "Components/TextBlock.h"

void UInteractionMessageWidget::ShowMessageText_Implementation(const FString& Message)
{
	if (GetVisibility() == ESlateVisibility::Collapsed)
		SetVisibility(ESlateVisibility::Visible);
	
	TextMessage->SetText(FText::FromString(Message));
	PlayFadeInAnimation();
}

void UInteractionMessageWidget::HideMessageText()
{
	PlayFadeOutAnimation();
}

void UInteractionMessageWidget::OnFadeAnimationEnd()
{
	if (bIsForwarded)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
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
}

void UInteractionMessageWidget::NativeDestruct()
{
	Super::NativeDestruct();
}
