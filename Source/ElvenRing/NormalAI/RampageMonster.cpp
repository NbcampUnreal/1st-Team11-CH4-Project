// Fill out your copyright notice in the Description page of Project Settings.
#include "ElvenRing/NormalAI/RampageMonster.h"
#include "ElvenRing/NormalAI/Rampage_AnimInstance.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "ElvenRing/NormalAI/NormalAIController.h"

#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

void ARampageMonster::BeginPlay()
{
	AUnitBase::BeginPlay(); //
	SetReplicates(true);
	//AttachDelegateToWidget(ECharacterType::RampageMonster);
	GetWorldTimerManager().SetTimer(UpdateHPBarTimer, this, &ARampageMonster::UpdateHPBar, 0.1f, true); // 0.5초마다 실행
}

ARampageMonster::ARampageMonster()
{
}

void ARampageMonster::MulticastIsHit(bool value, FVector HitLocation, FRotator HitRotation)
{
	URampage_AnimInstance* AnimInstance = Cast<URampage_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateHit(value);
		if (value == true)
		{
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(250, 300, 200),
			                                       HitLocation - FVector(0.0f, 0.0f, 320.0f),
			                                       FRotator(90.0f, 0.0f, 0.0f),
			                                       5.0f);
		}
	}
}

void ARampageMonster::MulticastIsAttack(bool value)
{
	URampage_AnimInstance* AnimInstance = Cast<URampage_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateAttack(value);
	}
}

void ARampageMonster::MulticastIsDeath(bool value)
{
	URampage_AnimInstance* AnimInstance = Cast<URampage_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateDeath(value);
	}

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->GravityScale = 0.0f; // 중력 제거
		UE_LOG(LogTemp, Warning, TEXT("Ram중력 제거 완료"));
	}

	if (UCapsuleComponent* Capsul = GetCapsuleComponent())
	{
		Capsul->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("Ram캡슐 제거성공"))
	}

	//HP바 타이머 정지, 위젯 제거
	GetWorldTimerManager().ClearTimer(UpdateHPBarTimer);

	if (UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetGameInstance()))
	{
		GameInstance->GetUIManager()->DestroyMonsterHpWidget(this);
	}
	if (HPWidgetComponent)
	{
		HPWidgetComponent->DestroyComponent();
	}
}



void ARampageMonster::OnRep_HealthChanged()
{
	Super::OnRep_HealthChanged();
	// 로컬 플레이어인 경우에만 UI 업데이트
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn() == this)
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
	else if (Cast<ARampageMonster>(this))
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
}
