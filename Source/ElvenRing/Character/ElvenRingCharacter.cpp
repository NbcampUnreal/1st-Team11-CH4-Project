// Fill out your copyright notice in the Description page of Project Settings.


#include "ElvenRingCharacter.h"

#include "../Core/ElvenRingGameMode.h"
#include "ElvenRingController.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "../Core/ElvenringGameInstance.h"
#include "ElvenRing/Interaction/InteractionComponent.h"
#include "array"
#include "GameFramework/SpringArmComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ElvenRing/UI/UIManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UniversalObjectLocators/AnimInstanceLocatorFragment.h"

AElvenRingCharacter::AElvenRingCharacter()
{
    //네트워크
    bReplicates = true;
    
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

    //테스트용
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw   = false;
    bUseControllerRotationRoll  = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    DodgeSpeed = 800.0f;
}



void AElvenRingCharacter::Multicast_PlayAttackAnimation_Implementation(UAnimMontage* Montage)
{
    if (Montage)
    {
        if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
        {
            bCanMove = false;
            AnimInstance->Montage_Play(Montage);
        }
    }
}

void AElvenRingCharacter::OnDeath()
{
    Super::OnDeath();
    bCanMove = false;
    if (HasAuthority())
    {
        Multicast_Death(DieMontage);
    }
}

void AElvenRingCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AElvenRingCharacter, IsSprint);
}

float AElvenRingCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
    if (bIsDie) return 0;
    if (Invincibility)
    {
        if (AElvenRingGameMode* GameMode = GetWorld()->GetAuthGameMode<AElvenRingGameMode>())
        {
            GameMode->RecordInvincible(EventInstigator, this, DamageAmount);
        }
        return 0;
    }
    float ActualDamage = 0.0f;
    if (bDefence)
    {
         ActualDamage = Super::TakeDamage(DamageAmount/2, DamageEvent, EventInstigator, DamageCauser);
    }
    else
    {
        ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    }
    
    if (HasAuthority() && CurHealth > 0)
    {
        if (!bDefence) Multicast_Hit(HitMontage);
    }
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();
    
    UNiagaraFunctionLibrary::SpawnSystemAtLocation
            (
            GetWorld(),              // 현재 월드 컨텍스트
            HitNiagara,    // 할당된 Niagara 이펙트 자산
            SpawnLocation,           // 스폰 위치
            SpawnRotation            // 스폰 회전 값
            );
    if (bIsAttacking) bCanMove = false;
    bIsAttacking = false;
    bCanCombo = false;
    CurrentWeapon->ResetDamagedActors();
    CurrentWeapon->DisableCollision();
    
    return ActualDamage;
}

void AElvenRingCharacter::HandleDeath()
{
    if (!HasAuthority())
    {
        return; 
    }

    AController* PC = GetController();
    if (!PC)
    {
        return;
    }
    
    if (UWorld* World = GetWorld())
    {
        if (AElvenRingGameMode* GM  = World->GetAuthGameMode<AElvenRingGameMode>())
        {
            GM->HandlePlayerDeath(PC);
            TArray<AActor*> Actors;
            GetAttachedActors(Actors,true);
            for (AActor* Actor : Actors)
            {
                Actor->Destroy();
            }
            Destroy();
        }
    }
}

void AElvenRingCharacter::Multicast_Heal_Implementation(UAnimMontage* Montage)
{
    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(Montage);
    }
}

void AElvenRingCharacter::Multicast_Hit_Implementation(UAnimMontage* Montage)
{
    if (Montage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(Montage);
        }
    }
}

void AElvenRingCharacter::Multicast_Death_Implementation(UAnimMontage* Montage)
{
    if (Montage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(Montage);
        }
    }

    bCanMove = false;
    if (IsLocallyControlled())
    {
        UGameplayStatics::PlaySoundAtLocation(this, DieSound, GetActorLocation());
        UElvenringGameInstance* Instance = Cast<UElvenringGameInstance>(GetGameInstance());
        Instance->GetUIManager()->ShowMessage("YOU DIE", EMessageType::SystemMessage);
    }
}


void AElvenRingCharacter::Multicast_PlayDodgeAnimation_Implementation(float _DodgeDuration)
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
    if (GetCharacterMovement()->IsFalling()) return;
    if (bJump) return;
    if (bdodge) return;
    if (!bIsAttacking)
    {

        bCanMove = false;
        bIsAttacking = true;
        AttackIndex = 1;
        if (HasAuthority())
        {
            Multicast_PlayAttackAnimation(AttackMontage1);
        }
    }
    // 이미 공격 중일 때, 콤보 입력 가능하면 연계
    else if (bIsAttacking && bCanCombo && AttackIndex < 3)
    {
        bCanMove = false;
        bCanCombo = false;

        if (GetWorldTimerManager().IsTimerActive(ComboTimerHandle))
        {
            GetWorldTimerManager().ClearTimer(ComboTimerHandle);
        }
        AttackIndex++;
        UAnimMontage* MontageToPlay = (AttackIndex == 2 ? AttackMontage2 : AttackMontage3);
        if (MontageToPlay && HasAuthority())
        {
            Multicast_PlayAttackAnimation(MontageToPlay);
        }
    }
}

void AElvenRingCharacter::OnDodgeInput(const FInputActionValue& Value)
{
    StartDodge(Value);
    
    // 만약 클라이언트라면 서버에도 dodge 입력을 알림
    if (!HasAuthority())
    {
        Server_OnDodgeInput(Value);
    }
}

void AElvenRingCharacter::Server_OnAttackInput_Implementation()
{
    OnAttackInput();
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

void AElvenRingCharacter::ServerStartSprint_Implementation()
{
    IsSprint = true;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

bool AElvenRingCharacter::ServerStartSprint_Validate()
{
    return true;
}

void AElvenRingCharacter::ServerStopSprint_Implementation()
{
    IsSprint = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
    }
}

bool AElvenRingCharacter::ServerStopSprint_Validate()
{
    return true;
}


void AElvenRingCharacter::Server_OnDodgeInput_Implementation(const FInputActionValue& Value)
{
    StartDodge(Value);
}

void AElvenRingCharacter::Server_Heal_Implementation()
{
    Heal(FInputActionValue());
}

void AElvenRingCharacter::OnAttackAnimationEnd()
{
    bCanCombo = true;
    GetWorldTimerManager().SetTimer(ComboTimerHandle, this, &AElvenRingCharacter::ComboEnd, 0.3f, false);
}

void AElvenRingCharacter::ComboEnd()
{
    if (bCanCombo)
    {
        bCanMove = true;
        ResetCombo();
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

                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Completed,
                    this,
                    &AElvenRingCharacter::MoveEnd
                );
            }
            
            if (PlayerController->HealAction)
            {
                // IA_Heal 액션 키를 "키를 누르면" Heal() 호출
                EnhancedInput->BindAction(
                    PlayerController->HealAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AElvenRingCharacter::Heal
                );

                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Completed,
                    this,
                    &AElvenRingCharacter::MoveEnd
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
                        ETriggerEvent::Completed,
                        this,
                        &AElvenRingCharacter::OnDodgeInput
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
                        &AElvenRingCharacter::Server_OnAttackInput
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

void AElvenRingCharacter::Heal(const FInputActionValue& value)
{
    if (bHealing) return;
    bHealing = true;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!HasAuthority())
    {
        Server_Heal();
        return;
    }
    bCanMove = false;
    CurHealth += 20;
    
    if (CurHealth > MaxHealth)
    {
        CurHealth = MaxHealth;
    }
    OnHpChanged.Broadcast(CurHealth, MaxHealth, 0);
    if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
    {
        Anim->Montage_Play(HealMontage);
    }
    Multicast_Heal(HealMontage);
}


void AElvenRingCharacter::SetbJump(bool _bJump)
{
    _bJump = bJump;
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
            //float MontageLength = DefenceMontage->GetPlayLength(); 일단 빼두자 언제 또 써야할지도 모른다.....
            
            AnimInstance->Montage_Play(DefenceMontage);
            AnimInstance->Montage_SetNextSection("End", "Start", DefenceMontage);
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
	MoveInput = value.Get<FVector2D>();
    if (!bCanMove) return;

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(ForwardDirection, MoveInput.X);
        
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(RightDirection, MoveInput.Y);
    }
}
void AElvenRingCharacter::MoveEnd(const FInputActionValue& value)
{
    MoveInput = {0,0};
}

void AElvenRingCharacter::StartJump(const FInputActionValue& value)
{
    if (bIsDodging) return;
    if (bDefence) return;
    if (bJump) return;
    if (bIsAttacking) return;
    if (value.Get<bool>())
    {
        Jump();
        bJump = true;
    }
}

void AElvenRingCharacter::SetBoolTrue()
{
    bJump = false;
}

void AElvenRingCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AElvenRingCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    
    GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, this, &AElvenRingCharacter::SetBoolTrue, 0.5f, false);
}
void AElvenRingCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AElvenRingCharacter::StartSprint(const FInputActionValue& value)
{
    if (HasAuthority())
    {
        IsSprint = true;
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
        }
    }
    else
    {
        ServerStartSprint();
    }
}

void AElvenRingCharacter::StopSprint(const FInputActionValue& value)
{
    if (HasAuthority())
    {
        IsSprint = false;
        if (GetCharacterMovement())
        {
            GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
        }
    }
    else
    {
        ServerStopSprint();
    }
}

void AElvenRingCharacter::StartDodge(const FInputActionValue& Value)
{
    if (GetCharacterMovement()->IsFalling() || bIsDodging)
    {
        return;
    }
    
    bCanMove = false;
    Invincibility = true;
    CurrentWeapon->DisableCollision();
    ResetCombo();

    FVector2D InputVector = Value.Get<FVector2D>();
    FVector DodgeDirection;
    if (!InputVector.IsNearlyZero())
    {
        const FRotator ControlRotation = GetControlRotation();
        const FRotator YawRotation(0, ControlRotation.Yaw, 0);
        const FVector CameraForward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector CameraRight   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        DodgeDirection = (InputVector.Y * CameraForward + InputVector.X * CameraRight).GetSafeNormal();
    }
    else
    {
        DodgeDirection = GetActorForwardVector();
    }

    DodgeStartLocation = GetActorLocation();
    DodgeTargetLocation = DodgeStartLocation + DodgeDirection * DodgeDistance;
    DodgeVelocity = DodgeDirection * (DodgeDistance / DodgeDuration);

    bdodge = true;
    bIsDodging = true;
    DodgeTime = 0.f;
    
    OriginalMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;
    GetCharacterMovement()->MaxWalkSpeed = 800.0f; 
    if (HasAuthority())
    {
        Multicast_PlayDodgeAnimation(DodgeDuration);
    }
    
    const float DodgeUpdate = 0.01f;
    GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &AElvenRingCharacter::UpdateDodge, DodgeUpdate, true);
    GetWorld()->GetTimerManager().SetTimer(DodgeStopTimerHandle, this, &AElvenRingCharacter::StopDodge, DodgeDuration, false);
    GetWorld()->GetTimerManager().SetTimer(DodgeStopTestTimerHandle, this, &AElvenRingCharacter::DodgeCollDown, DodgeDuration + DodgeCool, false);
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
    Invincibility = false;
    bCanMove = true;
    bdodge = false;
    CurrentWeapon->ResetDamagedActors();
    GetCharacterMovement()->MaxWalkSpeed = OriginalMaxSpeed;
}

void AElvenRingCharacter::DodgeCollDown()
{
    bIsDodging = false;
}

void AElvenRingCharacter::StartDefence(const FInputActionValue& value)
{
    bCanMove = false;;
    ResetCombo();
    if (bDefence)
    {
        return;
    }
    PlayDefenceAnimation(DefenceSpeed);
    bDefence = true;
}

void AElvenRingCharacter::StopDefence(const FInputActionValue& value)
{
    bCanMove = true;
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Stop(0.2f, DefenceMontage);
        bDefence = false;
        GetWorld()->GetTimerManager().ClearTimer(DefenceTimerHandle);
    }
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

    DodgeSpeed = 800.0f;
    
    if (HasAuthority())
    {
        CurHealth = MaxHealth;
    }
    if (IsLocallyControlled())
    {
        AttachDelegateToWidget(ECharacterType::Player);
        OnHpChanged.Broadcast(CurHealth,MaxHealth,0);
    }
    GetCharacterMovement()->RotationRate = FRotator(0.f, 780.f, 0.f);

    Tags.Add("Friendly");
    SprintSpeed = MoveSpeed * SprintSpeedMultiplier;
}