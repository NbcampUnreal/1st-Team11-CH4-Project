#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TitleMapParticle.generated.h"

UCLASS()
class ELVENRING_API ATitleMapParticle : public AActor
{
	GENERATED_BODY()
	
public:	
	ATitleMapParticle();

protected:
	virtual void BeginPlay() override;

public:	

};
