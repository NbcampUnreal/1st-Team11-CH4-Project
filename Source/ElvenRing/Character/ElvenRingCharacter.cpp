// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingCharacter.h"

#include "ElvenRingController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "ElvenRing/Interaction/InteractionComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AElvenRingCharacter::AElvenRingCharacter()
{
    //스프링 암
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->bUsePawnControlRotation = true;
	// 카메라
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent , USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
    //인터렉션 컴포
    InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));

    //공격 관련 함수 정의
    AttackIndex = 0;
    bIsAttacking = false;
    bCanCombo = false;
}

void AElvenRingCharacter::ToggleInput(bool _bInput)
{
    APlayerController* CharController = GetWorld()->GetFirstPlayerController();
    if (CharController)
    {
        if (_bInput)
        {
            EnableInput(CharController);
            SetActorHiddenInGame(false);
        }
        else
        {
            DisableInput(CharController);
            SetActorHiddenInGame(true);
        }

        // 소유한 액터들도 같이 보이거나 숨기도록 처리합니다.
        TArray<AActor*> OwnedActors;
        GetAttachedActors(OwnedActors); 
        for (AActor* OwnedActor : OwnedActors)
        {
            if (OwnedActor)
            {
                OwnedActor->SetActorHiddenInGame(!_bInput);
            }
        }
    }
}

//공격 관련 함수
void AElvenRingCharacter::OnAttackInput()
{
    if (!bIsAttacking)
    {
        if (CurrentWeapon)
        {
            CurrentWeapon->EnableCollision();
        }
        bIsAttacking = true;
        AttackIndex = 1;
        PlayAttackAnimation();
    }
    // 이미 공격 중일 때, 콤보 입력 가능하면 연계
    else if (bIsAttacking && bCanCombo && AttackIndex < 3)
    {
        bCanCombo = false;

        if (GetWorldTimerManager().IsTimerActive(ComboTimerHandle))
        {
            GetWorldTimerManager().ClearTimer(ComboTimerHandle);
        }
        AttackIndex++;
        PlayAttackAnimation();
    }
}

void AElvenRingCharacter::PlayAttackAnimation()
{
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    UAnimMontage* CurrentMontage = nullptr;
    switch (AttackIndex)
    {
    case 1:
        CurrentMontage = AttackMontage1;
        break;
    case 2:
        CurrentMontage = AttackMontage2;
        break;
    case 3:
        CurrentMontage = AttackMontage3;
        break;
    default:
        break;
    }
    if (CurrentMontage)
    {
        AnimInstance->Montage_Play(CurrentMontage);
    }
}

void AElvenRingCharacter::OnAttackAnimationEnd()
{
    bCanCombo = true;
    GetWorldTimerManager().SetTimer(ComboTimerHandle, this, &AElvenRingCharacter::ComboEnd, 0.5f, false);
}

void AElvenRingCharacter::ComboEnd()
{
    if (bCanCombo)
    {
        ResetCombo();
        if (CurrentWeapon)
        {
            CurrentWeapon->DisableCollision();
        }
    }
}

void AElvenRingCharacter::ResetCombo()
{
    bIsAttacking = false;
    bCanCombo = false;
    AttackIndex = 0;
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

                if (PlayerController->InteractAction)
                {
                    EnhancedInput->BindAction(
                        PlayerController->InteractAction,
                        ETriggerEvent::Started,
                        this,
                        &AElvenRingCharacter::Interact
                    );
                }
                
                if (PlayerController->AttackAction)
                {
                    // IA_Attack 액션 마우스가 "좌클릭할때" AttackAction() 호출
                    EnhancedInput->BindAction(
                        PlayerController->AttackAction,
                        ETriggerEvent::Triggered,
                        this,
                        &AElvenRingCharacter::OnAttackInput
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


void AElvenRingCharacter::PlayDefenceAnimation(float _DefenceSpeed)
{
    if (DefenceMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            ResetCombo();
            // delegate 바인딩 후 몽타주에 등록
            FOnMontageEnded MontageEndedDelegate;
            MontageEndedDelegate.BindUObject(this, &AElvenRingCharacter::OnDefenceMontageEnded);
            AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, DefenceMontage);

            //float MontageLength = DefenceMontage->GetPlayLength(); 일단 빼두자 언제 또 써야할지도 모른다.....
            
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
        GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
    }
}

void AElvenRingCharacter::StartDodge(const FInputActionValue& Value)
{
    if (bIsDodging)
    {
        return;
    }
    ResetCombo();
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
    GetWorld()->GetTimerManager().SetTimer(DodgeStopTestTimerHandle, this, &AElvenRingCharacter::DodgeCollDown, DodgeDuration+DodgeCool, false);
}

void AElvenRingCharacter::UpdateDodge()
{
    const float DodgeUpdate = 0.01f;
    float MoveDistance = DodgeVelocity.Size() * DodgeUpdate;
    
    AddMovementInput(DodgeVelocity.GetSafeNormal(), MoveDistance);
    DodgeTime += DodgeUpdate;
}

void AElvenRingCharacter::Interact(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Display, TEXT("Interact Pressed"));
    InteractionComponent->PerformInteract();
}

void AElvenRingCharacter::StopDodge()
{
    GetWorld()->GetTimerManager().ClearTimer(DodgeTimerHandle);
}

void AElvenRingCharacter::DodgeCollDown()
{
    bIsDodging = false;
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
void AElvenRingCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //sprint Test ksw
    if (GetCharacterMovement()->MaxWalkSpeed == SprintSpeed)
    {
        if (5.f < GetVelocity().Size())
            bSprint = true;
        else
            bSprint = false;
    }
    else
        bSprint = false;
}

void AElvenRingCharacter::BeginPlay()
{
    Super::BeginPlay();
    AttachDelegateToWidget(ECharacterType::Player);

    CurHealth = MaxHealth;
    
    SprintSpeed = MoveSpeed * SprintSpeedMultiplier;
    ToggleInput(bInput);
}