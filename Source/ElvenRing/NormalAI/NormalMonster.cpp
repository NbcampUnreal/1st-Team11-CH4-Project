#include "ElvenRing/NormalAI/NormalMonster.h"
#include "ElvenRing/NormalAI/NormalAIController.h"
#include "ElvenRing/NormalAI/Grux_AnimInstance.h"
#include "ElvenRing//Character/ElvenRingCharacter.h"
#include "ElvenRing/Core/ElvenringGameInstance.h"
#include "ElvenRing/UI/UIManager.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/WidgetComponent.h" //ksw
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "kismet/GameplayStatics.h"

ANormalMonster::ANormalMonster()
{
	MaxHealth = 1000;
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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent); // 🔹 캐릭터에 부착
	AudioComponent->bAutoActivate = false; // 기본적으로 자동 실행 비활성화

	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget")); //ksw
	HPWidgetComponent->SetupAttachment(RootComponent); //ksw
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World); //ksw
	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); //ksw
	HPWidgetComponent->SetTwoSided(true); //ksw
	// HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World); //ksw
	HPWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f)); //ksw

	PrimaryActorTick.bCanEverTick = false; //ksw
	bReplicates = true;
	GetMesh()->SetIsReplicated(true);
}

void ANormalMonster::BeginPlay()
{
	Super::BeginPlay();
	CurHealth = MaxHealth;
	SetReplicates(true);
	AttachDelegateToWidget(ECharacterType::NormalMonster); //ksw
	GetWorldTimerManager().SetTimer(UpdateHPBarTimer, this, &ANormalMonster::UpdateHPBar, 0.1f, true); // 0.5초마다 실행
}

void ANormalMonster::UpdateHPBar()
{
	if (HPWidgetComponent)
	{
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}
}

#pragma region 통신
void ANormalMonster::RPCIsHit_Implementation(bool value, AActor* DamageCauser)
{
	FVector HitLocation = GetActorLocation();
	FRotator HitRotation = DamageCauser->GetActorRotation();
	MulticastIsHit(value, HitLocation, HitRotation);
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
		GetController()->UnPossess();
		MulticastIsDeath(value);
	}
}

void ANormalMonster::MulticastIsHit_Implementation(bool value, FVector HitLocation, FRotator HitRotation)
{
	UGrux_AnimInstance* AnimInstance = Cast<UGrux_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->UpdateHit(value);
		if (value == true)
		{
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMaterial, FVector(250, 300, 200),
			                                       HitLocation - FVector(0.0f, 0.0f, 320.0f),
			                                       HitRotation,
			                                       5.0f);
		}
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

	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->GravityScale = 0.0f; // 중력 제거
	}

	if (UCapsuleComponent* Capsul = GetCapsuleComponent())
	{
		Capsul->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void ANormalMonster::OnRep_HealthChanged()
{
	Super::OnRep_HealthChanged();
	// 로컬 플레이어인 경우에만 UI 업데이트
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn() == this)
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
	else if (Cast<ANormalMonster>(this))
	{
		OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
	}
}

#pragma endregion

#pragma region 오디오
void ANormalMonster::PlaySound_Implementation(USoundBase* Sound)
{
	if (!Sound || !AudioComponent) return;
	AudioComponent->SetSound(Sound);
	AudioComponent->Play();
}

void ANormalMonster::PlayRandomSound_Implementation(ENormalMonsterSoundCategory Category)
{
	TArray<USoundBase*>* SoundArray = nullptr;

	switch (Category)
	{
	case ENormalMonsterSoundCategory::MoveSound:
		SoundArray = &MoveSounds;
		break;
	case ENormalMonsterSoundCategory::AttackSound:
		SoundArray = &AttackSounds;
		break;
	case ENormalMonsterSoundCategory::HitSound:
		SoundArray = &HitSounds;
		break;
	case ENormalMonsterSoundCategory::DeathSound:
		SoundArray = &DeathSounds;
		break;
	}

	if (SoundArray && SoundArray->Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, SoundArray->Num() - 1);
		USoundBase* SelectedSound = (*SoundArray)[RandomIndex];

		if (SelectedSound)
		{
			PlaySound(SelectedSound);
		}
	}
}
#pragma endregion

#pragma region 전투
float ANormalMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	InstanceIsHit = true;
	RPCIsHit(InstanceIsHit, DamageCauser);
	PlayRandomSound(ENormalMonsterSoundCategory::HitSound);

	return Damage;
}

void ANormalMonster::Attack(AActor* Target)
{
	if (Target)
	{
		TargetCharacterActor=Target;
		InstanceIsAttack = true;
		RPCIsAttack(InstanceIsAttack);
		PlayRandomSound(ENormalMonsterSoundCategory::AttackSound);
	}
}

void ANormalMonster::RealAttack(AActor* Target)
{
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
	PlayRandomSound(ENormalMonsterSoundCategory::DeathSound);
}

#pragma endregion


void ANormalMonster::SetWidget(UUserWidget* Widget)
{
	HPWidgetComponent->SetWidget(Widget);
}
