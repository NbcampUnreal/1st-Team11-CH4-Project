#include "CameraControllerComponent.h"
#include "EngineUtils.h"
#include "ElvenRing/Character/ElvenRingController.h"

void UCameraControllerComponent::ShakeCamera(TSubclassOf<UCameraShakeBase> CameraShakeClass, const float CameraShakeScale)
{
	if (!IsValid(CameraShakeClass)) return;

	for (AElvenRingController* Controller : TActorRange<AElvenRingController>(GetWorld()))
	{
		Controller->ClientStartCameraShake(CameraShakeClass, CameraShakeScale, ECameraShakePlaySpace::CameraLocal, FRotator::ZeroRotator);
	}
}

