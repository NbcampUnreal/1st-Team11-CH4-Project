// Fill out your copyright notice in the Description page of Project Settings.


//#include "UI/MessageWidgetBase.h"
#include "MessageWidgetBase.h"


void UMessageWidgetBase::ShowMessageText_Implementation(const FString& Message)
{
}

void UMessageWidgetBase::SetActiveWidget(bool bActive)
{
    if (bActive)
        SetVisibility(ESlateVisibility::Visible);
    else
        SetVisibility(ESlateVisibility::Collapsed);
}

void UMessageWidgetBase::PlayFadeAnimation()
{
    if (FadeAnim)
    {
        OnFadeFinishedDelegate.Unbind();
        OnFadeFinishedDelegate.BindDynamic(this, &UMessageWidgetBase::OnFadeAnimationFinished);

        UnbindAllFromAnimationFinished(FadeAnim);
        BindToAnimationFinished(FadeAnim, OnFadeFinishedDelegate);

        PlayAnimation(FadeAnim);
    }
}

void UMessageWidgetBase::OnFadeAnimationFinished()
{
    if(bCollapsed)
         SetVisibility(ESlateVisibility::Collapsed);
}
