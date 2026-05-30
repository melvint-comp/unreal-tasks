// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class P6_USERINTERFACES_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	UPROPERTY(EditAnywhere)
    int MaxAmmo = 3;
    
    UPROPERTY(EditAnywhere)
    int CurrentAmmo = 3;
	
	UPROPERTY(EditAnywhere)
	bool bReloading = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere)
	float RagdollTime = 5;
	FTimerHandle RagdollTimerHandle;
	void Ragdoll();
	UFUNCTION()
	void StopRagdoll();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100;

	void DealDamage(float Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootingDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FieldOfView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* BulletOffset;

	bool bCanShoot;
	float ReloadTimer;

	bool bIsRagdolling = false;
	
	bool Shoot(AActor* ShootTarget);
};
