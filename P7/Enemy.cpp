// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy.h"
#include "BrainComponent.h"
#include "EnemyAIController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootingDelay = 5.0f;
	bCanShoot = true;
	FieldOfView = 45.0f;
	ShootRange = 1000.0f;

	BulletOffset = CreateDefaultSubobject<USceneComponent>("BulletOffset");
	BulletOffset->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Target = GetWorld()->GetFirstPlayerController()->GetCharacter();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (!bCanShoot &&  !bIsRagdolling)
	// {
	// 	ReloadTimer += DeltaTime;
	// 	if (ReloadTimer >= ShootingDelay)
	// 	{
	// 		bCanShoot = true;
	// 		ReloadTimer = 0.0f;
	// 	}
	// }
	//
	// if (!Target)
	// 	return;
	// if (!ProjectileClass)
	// 	return;
	//
	// if (bCanShoot && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) <= ShootRange)
	// {
	// 	FVector Location = GetActorLocation();
	// 	FVector Direction = Target->GetActorLocation() - Location;
	// 	Direction.Normalize();
	// 	FVector Forward = GetActorForwardVector();
 //  
	// 	float RotationAngleRad = FMath::Acos(Forward.Dot(Direction));
	// 	float RotationAngleDeg = FMath::RadiansToDegrees(RotationAngleRad);
	// 	
	// 	if (RotationAngleDeg <= FieldOfView)
	// 	{
	// 		FActorSpawnParameters SpawnParam = FActorSpawnParameters();
	// 		SpawnParam.bNoFail = false;
	// 		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
 //     
	// 		FVector SpawnLocation = BulletOffset->GetComponentLocation();
	// 		FRotator SpawnRotation = Direction.Rotation();
	// 		AProjectileActor* Projectile = Cast<AProjectileActor>(GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation, SpawnParam));
	//
	// 		if (Projectile)
	// 		{
	// 			Projectile->Parent = this;
	// 			bCanShoot = false;
	// 		}
	// 	}
	// }
	
	if(bReloading)
	{
		ReloadTimer += DeltaTime;

		if(ReloadTimer >= 5.0f)
		{
			CurrentAmmo = MaxAmmo;
			ReloadTimer = 0;
			bReloading = false;
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::Ragdoll()
{
	Cast<AEnemyAIController>(GetController())->BrainComponent->PauseLogic("Ragdolling!");

	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AEnemy::StopRagdoll, RagdollTime, false);
	bIsRagdolling = true;
}

void AEnemy::StopRagdoll()
{
	Cast<AEnemyAIController>(GetController())->BrainComponent->ResumeLogic("Moving Again!");
	
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("pelvis"));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0, -90), FRotator(0, -90,0));
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	bIsRagdolling = false;
}

void AEnemy::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	if (CurrentHealth <= 0)
	{
		Destroy();
	}
}

bool AEnemy::Shoot(AActor* ShootTarget)
{
	if(CurrentAmmo <= 0)
	{
		return false;
	}
	
	if (!ShootTarget)
		return false;
	if (!ProjectileClass)
		return false;
	
	FVector Direction = ShootTarget->GetActorLocation() - BulletOffset->GetComponentLocation();
	Direction.Normalize();
	
	FActorSpawnParameters SpawnParam = FActorSpawnParameters();
	SpawnParam.bNoFail = false;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = BulletOffset->GetComponentLocation();
	FRotator SpawnRotation = Direction.Rotation();
	
	AProjectileActor* Projectile = Cast<AProjectileActor>(GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &SpawnRotation, SpawnParam));
	if (Projectile)
	{
		Projectile->Parent = this;
		bCanShoot = false;
		CurrentAmmo--;
		if(CurrentAmmo <= 0)
		{
			bReloading = true;
		}
	}
	return true;
}