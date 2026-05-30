// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InputActionValue.h"
#include "PlatformCharacter.generated.h"

/**
 * 
 */
UCLASS()
class P9_2DGAME_API APlatformCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
public:
    APlatformCharacter();
	
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* SideViewCamera;
        
	// Enhanced Input
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* JumpAction;
	
	// Animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UPaperFlipbook* JumpAnimation;
    
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    
    // Input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    // Movement functions
    void Move(const FInputActionValue& Value);
    void StartJump();
    void StopJump();
    void UpdateAnimation();
    
	float CoyoteTimeDuration = 0.15f;
	float LastGroundedTime = 0.0f;
};
