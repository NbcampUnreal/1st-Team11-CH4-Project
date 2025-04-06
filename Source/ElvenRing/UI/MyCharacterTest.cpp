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
        //HPWidgetComponent->SetDrawSize(FVector2D(50.f, 10.f)); // 초기 크기
        HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
        HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 80.f)); // 머리 위에 위치
        HPWidgetComponent->SetTwoSided(true);
        HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
        HPWidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
        HPWidgetComponent->SetDrawAtDesiredSize(true); // 위젯 자체 사이즈 기준으로 그리기
        //HPWidgetComponent->SetWidgetClass(HpBarWidgetClass); // WBP_MonsterHpBar
   
    }

}

// Called when the game starts or when spawned
void AMyCharacterTest::BeginPlay()
{
	Super::BeginPlay();
    UMonsterWidget* Uiwedget = Cast<UMonsterWidget>(HPWidgetComponent->GetUserWidgetObject());
    Uiwedget->SetUiSize(FVector2D(0.35f), FVector2D(0.f, 40.f));//Uiwedget->SetUiSize(0.4f);
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

