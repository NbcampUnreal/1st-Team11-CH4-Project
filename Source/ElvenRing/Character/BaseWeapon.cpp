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
	CollisionBox->SetGenerateOverlapEvents(true);


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
	AActor* RawOwner = GetOwner();
	if (RawOwner)
	{
		AUnitBase* OwnerCharacter = Cast<AUnitBase>(RawOwner);
		if (OwnerCharacter)
		{
			AttackPower = OwnerCharacter->GetAttackPower();
		}
	}
	if (OtherActor && OtherActor->IsOwnedBy(GetOwner()))
	{
		return;
	}

	AController* InstigatorController = GetInstigatorController();

	UGameplayStatics::ApplyDamage(OtherActor, AttackPower, InstigatorController, this, UDamageType::StaticClass());
}

