// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "ElvenRingCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameFramework/Controller.h"
// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(false);
}

void ABaseWeapon::EnableCollision()
{
	CollisionBox->SetGenerateOverlapEvents(true);
}

void ABaseWeapon::DisableCollision()
{
	CollisionBox->SetGenerateOverlapEvents(false);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABaseWeapon::OnOverlapBegin);
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	AElvenRingCharacter * Char = Cast<AElvenRingCharacter>(OtherActor);
	// 오너 캐릭터의 데미지를 가져오는 과정
	//근데 이거 데미지 변경될때만 하면 되는거 아닌가 오버랩 될때마다 해야함??
	if (!HasAuthority()) return;
	AActor* RawOwner = GetOwner();
	if (RawOwner)
	{
		AUnitBase* OwnerCharacter = Cast<AUnitBase>(RawOwner);
		if (OwnerCharacter)
         		{
         			AttackPower = OwnerCharacter->GetAttackPower();
         		}
	}
	// 해당 웨폰을 끼고 있는 캐릭터임
	if (OtherActor && OtherActor->IsOwnedBy(GetOwner()))
	{
		return;
	}
	// 이번 사이클에서 이미 데미지 입음
	if (OtherActor && DamagedActors.Contains(OtherActor))
	{
		return;
	}
	// 해당 캐릭터는 프랜들리(아군)임
	if (OtherActor->Tags.Contains("Friendly"))
	{
		return;
	}
	if (Char != nullptr)
	{
		int hp = Char->GetCurHealth();
		UE_LOG(LogTemp, Warning, TEXT("공격 대상: %s, 남은 체력: %d"), *OtherActor->GetName(), hp);
	}
	AController* InstigatorController = GetInstigatorController();
	// 클라이언트일 경우 서버로 요청 by 백의현
	if (!HasAuthority())
	{
		ServerApplyDamage(OtherActor, AttackPower, InstigatorController, this);
	}
	//UGameplayStatics::ApplyDamage(OtherActor, AttackPower, InstigatorController, this, UDamageType::StaticClass()); 백의현: 이거대신에
	ServerApplyDamage(OtherActor, AttackPower, InstigatorController, this); //이렇게 변경
	DamagedActors.Add(OtherActor);
	MulticastPlayAttackSound(GetActorLocation());
}

void ABaseWeapon::MulticastPlayAttackSound_Implementation(FVector Location)
{
	if (AttackSoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackSoundCue, Location);
	}
}

//by 백의현
void ABaseWeapon::ServerApplyDamage_Implementation(AActor* Target, float DamageAmount,
                                                   AController* InstigatorController, AActor* DamageCauser)
{
	UGameplayStatics::ApplyDamage(Target, DamageAmount, InstigatorController, DamageCauser, UDamageType::StaticClass());
}

void ABaseWeapon::ResetDamagedActors()
{
	DamagedActors.Empty();
}
