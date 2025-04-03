// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingCharacter.h"

#include "ElvenRingController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AElvenRingCharacter::AElvenRingCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent , USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}
void AElvenRingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (AElvenRingController* PlayerController = Cast<AElvenRingController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AElvenRingCharacter::Move
                );
            }
            
            if (PlayerController->JumpAction)
            {
                // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AElvenRingCharacter::StartJump
                );
                
                // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AElvenRingCharacter::StopJump
                );
            }
            
            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AElvenRingCharacter::Look
                );
            }
            
            if (PlayerController->SprintAction)
            {
                // IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered, 
                    this, 
                    &AElvenRingCharacter::StartSprint
                );
                // IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction, 
                    ETriggerEvent::Completed, 
                    this, 
                    &AElvenRingCharacter::StopSprint
                );

                
                if (PlayerController->DodgeAction)
                {
                    // IA_Dodge 액션 키를 "눌렀을 때" DodgeAction() 호출
                    EnhancedInput->BindAction(
                        PlayerController->DodgeAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::StartDodge
                    );
                }
                
                /*if (PlayerController->AttackAction)
                {
                    // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                    EnhancedInput->BindAction(
                        PlayerController->LookAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::Look
                    ); */
                
                /*if (PlayerController->DefenceAction)
                {
                    // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                    EnhancedInput->BindAction(
                        PlayerController->LookAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::Look
                    ); 
                }*/
            
            }    
        }
    }
}

void AElvenRingCharacter::Move(const FInputActionValue& value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AElvenRingCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void AElvenRingCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AElvenRingCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AElvenRingCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AElvenRingCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void AElvenRingCharacter::StartDodge(const FInputActionValue& value)
{
    if (bIsDodging)
        {
        return;
        }
    FVector DodgeDirection = GetLastMovementInputVector(); 
    if (DodgeDirection.IsNearlyZero())
    {
        // 입력 없을때 정면으로 구르기
        DodgeDirection = GetActorForwardVector();
    }
    DodgeDirection.Normalize();
    DodgeDirection.Z = 0;
    LaunchCharacter(DodgeDirection * DodgeStrength, true, true);

    bIsDodging = true;

    /*
     애니메이션 코드 추가할 자리
     필요없으면 안할거같음
     */

    // DodgeDuration 후에 구르기 상태를 종료하기 위해 타이머 설정
    GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &AElvenRingCharacter::StopDodge, DodgeDuration, false);
}

void AElvenRingCharacter::StopDodge()
{
    bIsDodging = false;
}
