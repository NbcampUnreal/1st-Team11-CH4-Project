#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaitingRoomPlayerCardsRT.generated.h"

class APlayerCard;
UCLASS()
class ELVENRING_API AWaitingRoomPlayerCardsRT : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaitingRoomPlayerCardsRT();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* SceneCapture;

	TArray<APlayerCard*> PlayerCards;

	UFUNCTION(BlueprintCallable,Category = "PlayerCard")
	void SetName(FText Name, int32 Idx);

	void ConnectOpenPlayerCard(int Index, bool bMyOrder = false);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCard")
	TSubclassOf<APlayerCard> PlayerCardClass;
	virtual void BeginPlay() override;

	void TempCreatePlayerCard(); //임시 Test 생성용 곧 지울것!!2250414

	

private:	
	FTimerHandle TimerHandle;
};
