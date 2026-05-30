// Copyright Epic Games, Inc. All Rights Reserved.

#include "P6_UserInterfacesCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "InGameUI.h"
#include "Enemy.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "P6_UserInterfaces.h"
#include "P6_UserInterfacesGameState.h"

AP6_UserInterfacesCharacter::AP6_UserInterfacesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AP6_UserInterfacesCharacter::StartStaminaRegen()
{
	if (!bIsSprinting)
	{
		bIsStaminaRegen = true;
	}
}

void AP6_UserInterfacesCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bIsSprinting)
	{
		ReduceStamina(StaminaRate * DeltaSeconds);
	}
	if (bIsStaminaRegen)
	{
		ReduceStamina(-StaminaRate * DeltaSeconds * 2);
	}
	if (CurrentPunchTimer < PunchCooldown)
	{
		CurrentPunchTimer += DeltaSeconds;
		if (InGameUI)
		{
			InGameUI->UpdateValues();
		}
	}
}

void AP6_UserInterfacesCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (InGameUIClass)
	{
		InGameUI = Cast<UInGameUI>(CreateWidget(GetGameInstance(), InGameUIClass));
		InGameUI->Player = this;
		InGameUI->UpdateValues();
		InGameUI->AddToViewport();
	}
}

void AP6_UserInterfacesCharacter::ReduceStamina(float Amount)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0, MaxStamina);
	if (CurrentStamina == 0) {
		SprintEnd();
	}
	if (CurrentStamina == MaxStamina) {
		bIsStaminaRegen = false;
	}
	if (InGameUI)
	{
		InGameUI->UpdateValues();
	}
}

void AP6_UserInterfacesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AP6_UserInterfacesCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AP6_UserInterfacesCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AP6_UserInterfacesCharacter::Look);
		
		// Punching
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Started, this, &AP6_UserInterfacesCharacter::Punch);
		
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AP6_UserInterfacesCharacter::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AP6_UserInterfacesCharacter::SprintEnd);

	}
	else
	{
		UE_LOG(LogP6_UserInterfaces, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AP6_UserInterfacesCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AP6_UserInterfacesCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AP6_UserInterfacesCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AP6_UserInterfacesCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AP6_UserInterfacesCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AP6_UserInterfacesCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AP6_UserInterfacesCharacter::Punch()
{
	if (CurrentPunchTimer >= PunchCooldown)
	{
		CurrentPunchTimer = 0;
		
		TArray<FHitResult> HitResults;
		const FVector Start = GetActorLocation();
		const FVector End = Start + GetActorForwardVector() * PunchDistance;
		const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(PunchDistance));
		const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), ECC_WorldDynamic, CubeShape);

		TArray<AActor*> HitThisPunch;
		TArray<AActor*> HitEnemies;
		
		for (FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor() != this && !HitThisPunch.Contains(HitResult.GetActor()))
			{
				HitThisPunch.Add(HitResult.GetActor());
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
				if (HitEnemy)
				{
					HitEnemy->Ragdoll();
					FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
					LaunchDirection.Normalize();
					LaunchDirection *= 3;
					LaunchDirection += FVector::UpVector;
					HitEnemy->GetMesh()->AddImpulse(LaunchDirection * PunchPower);
					HitEnemy->DealDamage(PunchDamage);
				}
				if (HitEnemy && !HitEnemies.Contains(HitEnemy))
				{
					HitEnemies.Add(HitEnemy);
					if (HitTextClass)
					{
						UHitTextUI* SpawnedHit = Cast<UHitTextUI>(CreateWidget(GetGameInstance(), HitTextClass));

						UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), HitEnemy->GetMesh()->GetComponentLocation(), SpawnedHit->CurrentLocation);

						SpawnedHit->HitText->SetText(FText::FromString(SpawnedHit->WordList[FMath::RandRange(0, SpawnedHit->WordList.Num() - 1)]));
						SpawnedHit->TargetLocation += SpawnedHit->CurrentLocation;
						SpawnedHit->AddToViewport();
					}
				}
			}
		}
	}
}
	


void AP6_UserInterfacesCharacter::SprintStart()
{
	if (!bIsSprinting)
	{
		if (CurrentStamina > 0)
		{
			GetCharacterMovement()->MaxWalkSpeed += SprintAmount;
			bIsSprinting = true;
			bIsStaminaRegen = false;
		}
	}
}

void AP6_UserInterfacesCharacter::SprintEnd()
{
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed -= SprintAmount;
		bIsSprinting = false;
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle,
	   this, &AP6_UserInterfacesCharacter::StartStaminaRegen,
	   StaminaRefreshCooldown);
	}
}

void AP6_UserInterfacesCharacter::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);	
	
	if (CurrentHealth <= 0)
	{  
		AP6_UserInterfacesGameState* GameState = Cast<AP6_UserInterfacesGameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			GameState->Score(true);
		}
		
		CurrentHealth = MaxHealth;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
	
	if (InGameUI)
	{
		InGameUI->UpdateValues();
	}
}
