#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaitingRoomPlayerCardsRT.generated.h"

class APlayerCard;
class UTextureRenderTarget2D;
class AWaitingRoomPlayerController;
UCLASS()
class ELVENRING_API AWaitingRoomPlayerCardsRT : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaitingRoomPlayerCardsRT();




	void OnUpdatePlayerName(int idx, FString Name);

	AWaitingRoomPlayerController* Controller;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USceneCaptureComponent2D* SceneCapture;

	TArray<APlayerCard*> PlayerCards;

	UFUNCTION(BlueprintCallable, Category = "PlayerCard")
	void SetGlowPoworDir(float Dir, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "RenderTarget")
	UTextureRenderTarget2D* GetRenderTarget();

	UPROPERTY(VisibleAnywhere,Category = "camera")
	class UCameraComponent* SceneCamera;

	//UFUNCTION(Server, Reliable,BlueprintCallable,Category = "PlayerCard")
	void SetName(FText Name);

	void ConnectOpenPlayerCard(int32 Index, bool bMyOrder = false);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerCard")
	TSubclassOf<APlayerCard> PlayerCardClass;
	virtual void BeginPlay() override;

	void TempCreatePlayerCard(); //임시 Test 생성용 곧 지울것!!2250414

private:	
	FTimerHandle TimerHandle;

	bool bTempInit = false;

	UTextureRenderTarget2D* MyRenderTarget;
};
