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
                
                if (PlayerController->AttackAction)
                {
                    // IA_Attack 액션 마우스가 "좌클릭할때" AttackAction() 호출
                    EnhancedInput->BindAction(
                        PlayerController->AttackAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::StartAttack
                    );
                }
                if (PlayerController->DefenceAction)
                {
                    // IA_Defence 액션 마우스가 "우클릭할 때" DefenceAction() 호출
                    EnhancedInput->BindAction(
                        PlayerController->DefenceAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::StartDefence
                    ); 
                }
                if (PlayerController->EndDefenceAction)
                {
                    // IA_Defence 액션 마우스가 "우클릭을 땔때" EndDefenceAction() 호출
                    EnhancedInput->BindAction(
                        PlayerController->EndDefenceAction,
                        ETriggerEvent::Completed,
                        this,
                        &AElvenRingCharacter::StopDefence
                    );
                }
            }    
        }
    }
}

void AElvenRingCharacter::PlayDodgeAnimation(float _DodgeDuration)
{
    if (DodgeMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            float MontageLength = DodgeMontage->GetPlayLength();
            float PlayRate = MontageLength / _DodgeDuration;
            AnimInstance->Montage_Play(DodgeMontage,PlayRate);
        }
    }
}

void AElvenRingCharacter::PlayAttackAnimation(float _AttackSpeed)
{
    if (AttackMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            float MontageLength = AttackMontage->GetPlayLength();
            float PlayRate = MontageLength / _AttackSpeed;
            AnimInstance->Montage_Play(AttackMontage,PlayRate);
        }
    }
}

void AElvenRingCharacter::PlayDefenceAnimation(float _DefenceSpeed)
{
    if (DefenceMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            // delegate 바인딩 후 몽타주에 등록
            FOnMontageEnded MontageEndedDelegate;
            MontageEndedDelegate.BindUObject(this, &AElvenRingCharacter::OnDefenceMontageEnded);
            AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DefenceMontage);

            float MontageLength = DefenceMontage->GetPlayLength();
            // PlayRate는 이제 필요 없으므로 기본값으로 사용
            AnimInstance->Montage_Play(DefenceMontage);
        }
    }
}

void AElvenRingCharacter::OnDefenceMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage == DefenceMontage)
    {
        // 특정 조건(예: 중단되지 않은 경우)을 체크한 후 다시 재생
        PlayDefenceAnimation(DefenceSpeed);
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

void AElvenRingCharacter::StartDodge(const FInputActionValue& Value)
{
    if (bIsDodging)
    {
        return;
    }
    
    FVector DodgeDirection = GetLastMovementInputVector();
    if (DodgeDirection.IsNearlyZero())
    {
        DodgeDirection = GetActorForwardVector();
    }
    DodgeDirection.Normalize();
    DodgeDirection.Z = 0;
    
    DodgeStartLocation = GetActorLocation();
    DodgeTargetLocation = DodgeStartLocation + DodgeDirection * DodgeDistance;
    
    DodgeVelocity = DodgeDirection * (DodgeDistance / DodgeDuration);
    
    bIsDodging = true;
    DodgeTime = 0.f;
    PlayDodgeAnimation(DodgeDuration);
    
    const float DodgeUpdate = 0.01f;
    GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &AElvenRingCharacter::UpdateDodge, DodgeUpdate, true);
    GetWorld()->GetTimerManager().SetTimer(DodgeStopTimerHandle, this, &AElvenRingCharacter::StopDodge, DodgeDuration, false);
    
}

void AElvenRingCharacter::UpdateDodge()
{
    const float DodgeUpdate = 0.01f;
    float MoveDistance = DodgeVelocity.Size() * DodgeUpdate;
    
    AddMovementInput(DodgeVelocity.GetSafeNormal(), MoveDistance);
    DodgeTime += DodgeUpdate;
}

void AElvenRingCharacter::StopDodge()
{
    bIsDodging = false;
    GetWorld()->GetTimerManager().ClearTimer(DodgeTimerHandle);
}
void AElvenRingCharacter::StartAttack(const FInputActionValue& value)
{
    if (bAttack)
    {
        return;
    }
    PlayAttackAnimation(AttackSpeed);
    bAttack = true;
    
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AElvenRingCharacter::StopAttack, AttackSpeed, false);
}

void AElvenRingCharacter::StopAttack()
{
    bAttack = false;
    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}

void AElvenRingCharacter::StartDefence(const FInputActionValue& value)
{
    if (bDefence)
    {
        return;
    }
    PlayDefenceAnimation(DefenceSpeed);
    bDefence = true;
}

void AElvenRingCharacter::StopDefence(const FInputActionValue& value)
{
    bDefence = false;
    GetWorld()->GetTimerManager().ClearTimer(DefenceTimerHandle);
}
