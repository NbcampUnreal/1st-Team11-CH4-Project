// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterTest.h"
#include "Components/WidgetComponent.h"
#include "PlayerMainUi.h"
#include "MonsterWidget.h"

// Sets default values
AMyCharacterTest::AMyCharacterTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   // if (IsLocallyControlled()) // 또는 GetLocalRole() == ROLE_AutonomousProxy
    {
        // 위젯 컴포넌트 생성 및 설정
        HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidget"));
        HPWidgetComponent->SetupAttachment(RootComponent);
        HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
        HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f)); // 머리 위에 위치
        HPWidgetComponent->SetTwoSided(true);
        HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
        HPWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
    }

}

// Called when the game starts or when spawned
void AMyCharacterTest::BeginPlay()
{
	Super::BeginPlay();
    UMonsterWidget* Uiwedget = Cast<UMonsterWidget>(HPWidgetComponent->GetUserWidgetObject());
    Uiwedget->SetUiSize(0.8f);
    Uiwedget->DecreaseHp(80.f,100.f);
}

// Called every frame
void AMyCharacterTest::Tick(float DeltaTime)
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

// Called to bind functionality to input
void AMyCharacterTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

