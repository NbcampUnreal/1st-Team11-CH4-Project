// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "ElvenRing/NormalAI/Grux_AnimInstance.h"
#include "ElvenRing//UI/MonsterWidget.h"//ksw
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"
#include "Net/UnrealNetwork.h"

#include "Components/WidgetComponent.h" //ksw
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"


ANormalMonster::ANormalMonster()
{
	MaxHealth = 100;
	CurHealth = MaxHealth;
	AttackPower = 10;
	MoveSpeed = 10;

	AttackDistance = 250.0f;
	AttackAngle = 60.0f;

	bCanAttack = true;
	bCanMove = true;
	MonsterIsHit = false;
	bIsDie = false;
	AIControllerClass = ANormalAIController::StaticClass();

	InstanceIsAttack = false;
	InstanceIsHit = false;
	InstanceIsDeath = false;

	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget")); //ksw
	HPWidgetComponent->SetupAttachment(RootComponent); //ksw
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World); //ksw
	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); //ksw
	HPWidgetComponent->SetTwoSided(true); //ksw
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World); //ksw
	HPWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); //ksw
	PrimaryActorTick.bCanEverTick = false; //ksw
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	AttachDelegateToWidget(ECharacterType::NormalMonster); //ksw
	GetWorldTimerManager().SetTimer(UpdateHPBarTimer, this, &ANormalMonster::UpdateHPBar, 0.1f, true); // 0.5초마다 실행
}

void ANormalMonster::UpdateHPBar()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FVector CamLoc = PC->PlayerCameraManager->GetCameraLocation();
		FVector MyLoc = HPWidgetComponent->GetComponentLocation();

		FRotator LookRot = (CamLoc - MyLoc).Rotation();
		HPWidgetComponent->SetWorldRotation(LookRot);
	}
}

void ANormalMonster::RPCIsHit_Implementation(bool value)
{
	
		MulticastIsHit(value);
	
}

void ANormalMonster::RPCIsAttack_Implementation(bool value)
{
	if (HasAuthority())
	{
		MulticastIsAttack(value);
	}
}

void ANormalMonster::RPCIsDeath_Implementation(bool value)
{
	if (HasAuthority())
	{
		MulticastIsDeath(value);
	}
}

void ANormalMonster::MulticastIsHit_Implementation(bool value)
{
	UGrux_AnimInstance* AnimInstance = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateHit(value);
		
	}
}

void ANormalMonster::MulticastIsAttack_Implementation(bool value)
{
	UGrux_AnimInstance* AnimInstance = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateAttack(value);
	}
}

void ANormalMonster::MulticastIsDeath_Implementation(bool value)
{
	UGrux_AnimInstance* AnimInstance = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateDeath(value);
	}
}

float ANormalMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	InstanceIsHit = true;
	RPCIsHit(InstanceIsHit);
	return Damage;
}

void ANormalMonster::Attack(AActor* Target)
{
	if (Target)
	{
		InstanceIsAttack = true;
		RPCIsAttack(InstanceIsAttack);
		FVector MonsterLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		FVector DirectionToTarget = (TargetLocation - MonsterLocation).GetSafeNormal();

		FVector MonsterForward = GetActorForwardVector();
		float DotProduct = FVector::DotProduct(MonsterForward, DirectionToTarget);
		float AngleDegrees = FMath::Acos(DotProduct) * (180.0f / PI);
		float Distance = FVector::Dist(MonsterLocation, TargetLocation);

		if (Distance <= AttackDistance && AngleDegrees <= AttackAngle) // 120도 범위 (60도 좌우)
		{
			UGameplayStatics::ApplyDamage(Target, AttackPower, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void ANormalMonster::PlayerDetected(UObject* TargetCharacter)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

	BlackboardComp->SetValueAsBool(TEXT("PlayerDetectedKey"), true);
	BlackboardComp->SetValueAsBool(TEXT("IsWatingKey"), false);
	BlackboardComp->SetValueAsObject(TEXT("TargetActor"), (TargetCharacter));
}

void ANormalMonster::OnDeath()
{
	Super::OnDeath();
	InstanceIsDeath = true;
	RPCIsDeath(InstanceIsDeath);

	// 콜리전 제거
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//HP바 타이머 정지, 위젯 제거
	GetWorldTimerManager().ClearTimer(UpdateHPBarTimer);
	UElvenringGameInstance* GameInstance = Cast<UElvenringGameInstance>(GetGameInstance());
	GameInstance->GetUIManager()->DestroyMonsterHpWidget(this);
	HPWidgetComponent->DestroyComponent();
	GetController()->UnPossess();
}

void ANormalMonster::SetWidget(UUserWidget* Widget)
{
	HPWidgetComponent->SetWidget(Widget);
}
