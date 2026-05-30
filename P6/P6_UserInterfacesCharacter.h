// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HitTextUI.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "P6_UserInterfacesCharacter.generated.h"

class UInGameUI;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AP6_UserInterfacesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AP6_UserInterfacesCharacter();	
	
	UPROPERTY(EditAnywhere)
    float MaxStamina = 100;
    UPROPERTY(EditAnywhere)
    float StaminaRate = 20;
    UPROPERTY(EditAnywhere)
    float StaminaRefreshCooldown = 2;
    UPROPERTY(EditAnywhere)
    float PunchCooldown = 0.5;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UInGameUI> InGameUIClass;
    UPROPERTY(EditAnywhere)
    UInGameUI* InGameUI;
    float CurrentStamina = MaxStamina;
    float CurrentPunchTimer = PunchCooldown;
    bool bIsSprinting = false;
    bool bIsStaminaRegen = false;
    FTimerHandle StaminaRegenHandle;
	
	UFUNCTION()
	void StartStaminaRegen();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	void ReduceStamina(float Amount);
	
	UPROPERTY(EditAnywhere) 
	TSubclassOf<UHitTextUI> HitTextClass;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	UPROPERTY(EditAnywhere)
	float SprintAmount = 500;
	UPROPERTY(EditAnywhere)
	float PunchPower = 100;
	UPROPERTY(EditAnywhere)
	float PunchDistance = 250;
	UPROPERTY(EditAnywhere)
	float PunchDamage = 50;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PunchAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;
	
	UFUNCTION()
	void Punch();
	UFUNCTION()
	void SprintStart();
	UFUNCTION()
	void SprintEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;
	
	void DealDamage(float Damage);

};

