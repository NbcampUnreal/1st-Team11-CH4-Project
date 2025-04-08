// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"

#include "ElvenRingCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
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
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오너 캐릭터의 데미지를 가져오는 과정
	//근데 이거 데미지 변경될때만 하면 되는거 아닌가 오버랩 될때마다 해야함??
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
	AController* InstigatorController = GetInstigatorController();

	UGameplayStatics::ApplyDamage(OtherActor, AttackPower, InstigatorController, this, UDamageType::StaticClass());
	DamagedActors.Add(OtherActor);
}
void ABaseWeapon::ResetDamagedActors()
{
	DamagedActors.Empty();
}