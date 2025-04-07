// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "Engine/DamageEvents.h"

#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"

ANormalMonster::ANormalMonster()
{
	MaxHealth = 100;
	CurHealth = MaxHealth;
	AttackPower = 10;
	MoveSpeed = 10;
	bCanAttack = true;
	bCanMove = true;
	bIsHit = false;
	bIsDie = false;

	AIControllerClass = ANormalAIController::StaticClass();
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
}

float ANormalMonster::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                 class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 처리
	CurHealth -= ActualDamage;
	UE_LOG(LogTemp, Warning, TEXT("몬스터 체력: %f"), CurHealth);
	PlayDamageAnim();
	if (CurHealth <= 0)
	{
		OnDeath(); // 체력이 0 이하이면 사망 처리
	}

	return ActualDamage;
}

void ANormalMonster::Attack(AActor* Target)
{
	if (Target)
	{
		UGameplayStatics::ApplyDamage(Target, AttackPower, GetController(), this, UDamageType::StaticClass());

		UE_LOG(LogTemp, Warning, TEXT("몬스터가 %f 데미지를 적용"), AttackPower);

		// 애니메이션 적용 (AnimSequence 타입)
		UAnimSequence* AttackAnim = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/ParagonGrux/Characters/Heroes/Grux/Animations/DoublePain.DoublePain'"));
		if (AttackAnim && GetMesh())
		{
			GetMesh()->PlayAnimation(AttackAnim, false);
		}
	}
}


void ANormalMonster::PlayDamageAnim()
{
	Super::PlayDamageAnim();

	// 애니메이션 로드
	UAnimSequence* DamageAnim = LoadObject<UAnimSequence>(
		nullptr, TEXT("/Script/Engine.AnimSequence'/Game/ParagonGrux/Characters/Heroes/Grux/Animations/HitReact_Front.HitReact_Front'"));

	if (DamageAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(DamageAnim, false); // 애니메이션 실행
	}
}

void ANormalMonster::PlayDeathAnim()
{
	Super::PlayDeathAnim();

	// 애니메이션 로드
	UAnimSequence* DeathAnim = LoadObject<UAnimSequence>(nullptr, TEXT("/Script/Engine.AnimSequence'/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Death_B.Death_B'"));

	if (DeathAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(DeathAnim, false);  // 애니메이션 실행
	}
}

void ANormalMonster::OnDeath()
{
	Super::OnDeath();
	PlayDeathAnim();

	
}

