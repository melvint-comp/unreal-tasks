// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PaperFlipbookComponent.h"
#include "PlatformCharacter.h"

APlatformCharacter::APlatformCharacter()
{
	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(25.0f, 48.0f);
	
	// Lock movement to X/Z (2D)
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f,1.0f, 0.0f));
	
	// Movement tuning
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 350.0f;
	GetCharacterMovement()->MaxWalkSpeed = 180.0f;
	GetCharacterMovement()->AirControl = 0.7f;
	
	// Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->bDoCollisionTest = false;
	
	// Camera
	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCamera->SetupAttachment(CameraBoom);
	SideViewCamera->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCamera->OrthoWidth = 400.0f;
	
	// Disable rotation
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	PrimaryActorTick.bCanEverTick = true;
}

void APlatformCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Add input mapping context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlatformCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetCharacterMovement()->IsMovingOnGround()) {
		LastGroundedTime = GetWorld()->GetTimeSeconds();
	}
	
	UpdateAnimation();
}

void APlatformCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Move);
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlatformCharacter::StartJump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlatformCharacter::StopJump);
}

void APlatformCharacter::Move(const FInputActionValue& Value)
{
	const float Direction = Value.Get<float>();
	AddMovementInput(FVector(1.f, 0.f, 0.f), Direction);
	
	// Flip sprite
	if (Direction != 0.f)
	{
		GetSprite()->SetRelativeScale3D(FVector(Direction > 0 ? 1.f : -1.f ,1.f ,1.f));
	}
}

void APlatformCharacter::StartJump()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (GetCharacterMovement()->IsMovingOnGround() ||
		(CurrentTime - LastGroundedTime <= CoyoteTimeDuration))
	{
		Jump();
		LastGroundedTime = -1000.0f;
	}
}

void APlatformCharacter::StopJump()
{
	StopJumping();
}

void APlatformCharacter::UpdateAnimation()
{
	FVector Velocity = GetVelocity();
    if (GetCharacterMovement()->IsFalling())
    {
		if (JumpAnimation)
			GetSprite()->SetFlipbook(JumpAnimation);
    }
    else if (FMath::Abs(Velocity.X) > 0.01f)
    {
	    if (RunAnimation)
			GetSprite()->SetFlipbook(RunAnimation);
    }
    else
    {
		if (IdleAnimation)
			GetSprite()->SetFlipbook(IdleAnimation);
    }
}
