// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"

#include "Components/CapsuleComponent.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "Engine/DamageEvents.h"

#include "ElvenRing/ElvenRing.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h" //ksw
#include "ElvenRing//UI/MonsterWidget.h"//ksw

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

	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget")); //ksw
	HPWidgetComponent->SetupAttachment(RootComponent);//ksw
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);//ksw
	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); //ksw
	HPWidgetComponent->SetTwoSided(true);//ksw
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);//ksw
	HPWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));//ksw
	PrimaryActorTick.bCanEverTick = true;//ksw
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	AttachDelegateToWidget(ECharacterType::NormalMonster);//ksw
	UMonsterWidget* Uiwedget = Cast<UMonsterWidget>(HPWidgetComponent->GetUserWidgetObject());//ksw
	if (Uiwedget) //순서중요! AttachDelegateToWidget() > SetWidget()로 hp위젯을 먼저 얻어와야함.
		Uiwedget->SetUiSize(FVector2D(0.8f), FVector2D(0.f, 0.5f));//ksw
}


float ANormalMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	 Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	return Damage;
}


void ANormalMonster::Attack(AActor* Target)
{
	if (Target)
	{
		UGameplayStatics::ApplyDamage(Target, AttackPower, GetController(), this, UDamageType::StaticClass());

		UE_LOG(LogTemp, Warning, TEXT("몬스터가 %f 데미지를 적용"), AttackPower);

		// 애니메이션 적용 (AnimSequence 타입)
		UAnimSequence* AttackAnim = LoadObject<UAnimSequence>(
			nullptr, TEXT(
				"/Script/Engine.AnimSequence'/Game/ParagonGrux/Characters/Heroes/Grux/Animations/DoublePain.DoublePain'"));
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
	UAnimSequence* DeathAnim = LoadObject<UAnimSequence>(
		nullptr, TEXT(
			"/Script/Engine.AnimSequence'/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Death_B.Death_B'"));

	if (DeathAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(DeathAnim, false); // 애니메이션 실행
	}
}

void ANormalMonster::OnDeath()
{
	Super::OnDeath();
	PlayDeathAnim();
	if (GetController())
	{
		GetController()->UnPossess();
	}

	// 콜리전 제거
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsDie = true;
	HPWidgetComponent->DestroyComponent();
}
void ANormalMonster::SetWidget(UUserWidget* Widget)
{
	HPWidgetComponent->SetWidget(Widget);
}
void ANormalMonster::Tick(float DeltaTime)//ksw
{
	Super::Tick(DeltaTime);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		FVector MyLoc = HPWidgetComponent->GetComponentLocation();

		FRotator LookRot = (CamLoc - MyLoc).Rotation();
		// LookRot.Pitch = 0.f;
		//LookRot.Roll = 0.f; // 수직 회전 제거해서 평면 유지

		HPWidgetComponent->SetWorldRotation(LookRot);
	}
}