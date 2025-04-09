// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "ElvenRing/NormalAI/Grux_AnimInstance.h"
#include "ElvenRing//UI/MonsterWidget.h"//ksw

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
	bIsHit = false;
	bIsDie = false;

	AIControllerClass = ANormalAIController::StaticClass();

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
	AttachDelegateToWidget(ECharacterType::NormalMonster); //ksw
	UMonsterWidget* Uiwedget = Cast<UMonsterWidget>(HPWidgetComponent->GetUserWidgetObject()); //ksw
	if (Uiwedget) //순서중요! AttachDelegateToWidget() > SetWidget()로 hp위젯을 먼저 얻어와야함.
	{
		Uiwedget->SetUiSize(FVector2D(0.8f), FVector2D(0.f, 0.5f)); //ksw
	}

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

float ANormalMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	bisHit = true;
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	UGrux_AnimInstance* Grux_Anim = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	Grux_Anim->HitAnim();
	bisHit=false;
	return Damage;
}

void ANormalMonster::Attack(AActor* Target)
{
	if (Target)
	{
		//애니메이션 실행
		UGrux_AnimInstance* Grux_Anim = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
		Grux_Anim->AttackAnim();

		
		FVector MonsterLocation = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		FVector DirectionToTarget = (TargetLocation - MonsterLocation).GetSafeNormal();

		FVector MonsterForward = GetActorForwardVector();
		float DotProduct = FVector::DotProduct(MonsterForward, DirectionToTarget);
		float AngleDegrees = FMath::Acos(DotProduct) * (180.0f / PI);

		float Distance = FVector::Dist(MonsterLocation, TargetLocation);

		if (Distance <= AttackDistance && AngleDegrees <= AttackAngle) // 120도 범위 (60도 좌우)
		{
			UE_LOG(LogTemp, Warning, TEXT("공격 성공"));
			UGameplayStatics::ApplyDamage(Target, AttackPower, GetController(), this, UDamageType::StaticClass());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("공격 실패"));
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

void ANormalMonster::PlayDeathAnim()
{
	Super::PlayDeathAnim();
	UGrux_AnimInstance* Grux_Anim = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	Grux_Anim->DeathAnim();
}

void ANormalMonster::OnDeath()
{
	Super::OnDeath();
	PlayDeathAnim();
	UGrux_AnimInstance* Grux_Anim = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());


	GetController()->UnPossess();


	// 콜리전 제거
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsDie = true;
	GetWorldTimerManager().ClearTimer(UpdateHPBarTimer);
	HPWidgetComponent->DestroyComponent();
}

void ANormalMonster::SetWidget(UUserWidget* Widget)
{
	HPWidgetComponent->SetWidget(Widget);
}
