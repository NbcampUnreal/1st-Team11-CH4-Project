#include "PlayerNameWidget.h"
#include "Components/TextBlock.h"

void UPlayerNameWidget::SetName(FText Name)
{
	TextName->SetText(Name);//FText::FromString(Name)
}
