// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class ELVENRING_API ADoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

	/** 문 열기, Lock되어 있으면 열리지 않는다. */
	UFUNCTION(BlueprintCallable)
	void Open();
	/** 문 닫기 */
	UFUNCTION(BlueprintCallable)
	void Close();
	/** 문 열기/닫기 토글 */
	UFUNCTION(BlueprintCallable)
	void Toggle();
	
protected:
	/** 열린 상태에서의 Transform*/
	UPROPERTY(EditAnywhere)
	FTransform OpenTransform;
	UPROPERTY(EditAnywhere)
	/** 닫힌 상태에서의 Transform*/
	FTransform CloseTransform;
	UPROPERTY(EditAnywhere)
	/** 문이 열리는 시간 */
	float OpenCloseDuration;
	/** 문 애니메이션 업데이트 주기 */
	UPROPERTY(EditAnywhere)
	float DoorMovementUpdateInterval;
	/** 현재 문 열림 진행도 */
	float CurrentAlpha;
	
	FTimerHandle OpenCloseTimerHandle;

	/** bIsOpen의 상태에 맞춰서 문 애니메이션 재생 시작 */
	void StartDoorMovement();
	/** Timer로 문 애니메이션 재생 */
	void UpdateDoorMovement();

public:
	UFUNCTION(BlueprintPure)
	bool IsOpen() const { return bIsOpen; }
	/** 잠금 설정, 잠금 상태이면 Open을 호출해도 열리지 않는다. */
	UFUNCTION(BlueprintCallable)
	void SetLocked(const bool bLocked) { bIsLocked = bLocked; }
	/** 잠금 해제 */
	UFUNCTION(BlueprintPure)
	bool IsLocked() const { return bIsLocked; }
	/** 열릴 수 있는 문 여부, Can Open일 경우 Close 상태에서 상호 작용되지 않는다. */
	UFUNCTION(BlueprintPure)
	bool CanOpen() const { return bCanOpen; }
	/** 닫힐 수 있는 문 여부, Can Close일 경우 Open 상태에서 상호 작용되지 않는다. */
	UFUNCTION(BlueprintPure)
	bool CanClose() const { return bCanClose; }

protected:
	UPROPERTY(EditAnywhere)
	bool bIsLocked;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_IsOpen)
	bool bIsOpen;
	UPROPERTY(EditAnywhere)
	bool bCanOpen;
	UPROPERTY(EditAnywhere)
	bool bCanClose;

	UPROPERTY(EditAnywhere)
	FString LockInteractText;
	UPROPERTY(EditAnywhere)
	FString OpenInteractText;
	UPROPERTY(EditAnywhere)
	FString CloseInteractText;
	const FString DefaultLockInteractText;
	const FString DefaultOpenInteractText;
	const FString DefaultCloseInteractText;
	
	UFUNCTION()
	void OnRep_IsOpen();
public:
	// IInteractable interface Start
	virtual FString GetInteractText() override;
	virtual void Interact(APlayerController* Player) override;
	virtual bool CanInteract() override;
	// IInteractable interface End

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> OpenSound;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CloseSound;
private:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
