#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELVENRING_API UCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void ShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeClass, const float CameraShakeScale);
};
