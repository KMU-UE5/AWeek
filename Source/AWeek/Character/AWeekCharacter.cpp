// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../Player/AWeekPlayerState.h"
#include "../Input/AWeekGameInput.h"

DEFINE_LOG_CATEGORY(AWeekCharacter);

AAWeekCharacter::AAWeekCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Get Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		RunToStop(TEXT("/Script/Engine.AnimMontage'/Game/Survival_Character/Demo/Characters/Mannequins/Animations/Manny/Sprint/AM_RunToStop.AM_RunToStop'"));

	if (RunToStop.Succeeded())
		mRunToStopMontage = RunToStop.Object;
}

// Called when the game starts or when spawned
void AAWeekCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		Subsystem->AddMappingContext(InputCDO->mContext, 0);
	}
}

// Called every frame
void AAWeekCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAWeekPlayerState* State = Cast<AAWeekPlayerState>(GetPlayerState());

	if (bSprint)
	{
		if (State->UseStamina(mSprintStaminaUsage * DeltaTime) == false || GetVelocity().Size() < 50)
		{
			SprintCompleted();
		}
		else
		{
			mSprintTime += DeltaTime;
		}
	}
}

// Called to bind functionality to input
void AAWeekCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (IsValid(EnhancedInput))
	{
		const UAWeekGameInput* InputCDO = GetDefault<UAWeekGameInput>();

		EnhancedInput->BindAction(InputCDO->mMove, ETriggerEvent::Triggered,
			this, &AAWeekCharacter::Move);

		EnhancedInput->BindAction(InputCDO->mLook, ETriggerEvent::Triggered,
			this, &AAWeekCharacter::Look);

		EnhancedInput->BindAction(InputCDO->mJump, ETriggerEvent::Started,
			this, &AAWeekCharacter::Jump);

		EnhancedInput->BindAction(InputCDO->mJump, ETriggerEvent::Completed,
			this, &ACharacter::StopJumping);

		EnhancedInput->BindAction(InputCDO->mSprint, ETriggerEvent::Started,
			this, &AAWeekCharacter::SprintStart);

		EnhancedInput->BindAction(InputCDO->mSprint, ETriggerEvent::Completed,
			this, &AAWeekCharacter::SprintCompleted);

		EnhancedInput->BindAction(InputCDO->mAttack, ETriggerEvent::Started,
			this, &AAWeekCharacter::Attack);
	}
}

void AAWeekCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAWeekCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAWeekCharacter::Jump()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(mRunToStopMontage))
	{
		Super::Jump();
	}
}

void AAWeekCharacter::Attack(const FInputActionValue& Value)
{

}

void AAWeekCharacter::SprintStart(const FInputActionValue& Value)
{
	AAWeekPlayerState* State = Cast<AAWeekPlayerState>(GetPlayerState());

	if (GetMovementComponent()->IsFalling() || GetVelocity().Size() < 50 || State->GetStamina() < mSprintMinimumStamina)
		return;
	GetCharacterMovement()->MaxWalkSpeed = mSprintSpeed;
	bSprint = true;
}

void AAWeekCharacter::SprintCompleted()
{
	GetCharacterMovement()->MaxWalkSpeed = mWalkSpeed;

	bool Fall = GetCharacterMovement()->IsFalling();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (!Fall && bSprint && mSprintTime > mRunToStopTime && GetVelocity().Size() >= 50)
		{
			AnimInstance->Montage_Play(mRunToStopMontage);
		}
	}

	mSprintTime = 0;
	bSprint = false;
}

void AAWeekCharacter::FootStepEffect(FName SocketName)
{
	FVector	Position = GetMesh()->GetSocketLocation(SocketName);
	UNiagaraSystem* FootStepVFX = LoadObject<UNiagaraSystem>(GetWorld(), TEXT("/Script/Niagara.NiagaraSystem'/Game/A_Surface_Footstep/Niagara_FX/ParticleSystems/PSN_General1_Surface.PSN_General1_Surface'"));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FootStepVFX, Position);
}
