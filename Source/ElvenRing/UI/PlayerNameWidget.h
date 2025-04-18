#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"

class UTextBlock;
UCLASS()
class ELVENRING_API UPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextName;

	UFUNCTION(BlueprintCallable,Category = "UI")
	void SetName(FText Name);
	UFUNCTION(BlueprintCallable, Category = "UI")
	FText GetTextName();
};
