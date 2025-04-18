#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCard.generated.h"

class UPlayerNameWidget;
class UWidgetComponent;
UCLASS()
class ELVENRING_API APlayerCard : public AActor
{
	GENERATED_BODY()
	
public:	

	APlayerCard();

	UFUNCTION(BlueprintCallable, Category = "PlayerCard")
	void SetIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "PlayerCard")
	void SetName(FText Name);
	FText GetPlayerName();
protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "PlayerCard")
	int32 PlayerIndex;

	UPROPERTY(VisibleAnywhere, Category = "PlayerCard")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere,Category = "PlayerCard")
	USkeletalMeshComponent* PlayerCardMesh;

	//UPROPERTY(VisibleAnywhere, Category = "PlayerCard")
	//UPlayerNameWidget* NickNameWidget;

	UPROPERTY(VisibleAnywhere, Category = "PlayerCard")
	UWidgetComponent* NameWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCard")
	FVector NameWidgetPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCard")
	FRotator NameWidgetRot;

	FName BoneName;
	virtual void BeginPlay() override;
public:	

	virtual void Tick(float DeltaTime) override;
private:

	void PlayerCardEffect();
};
