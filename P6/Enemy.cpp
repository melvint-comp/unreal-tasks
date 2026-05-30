// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/WidgetComponent.h"
#include "EnemyFloatingHealthBar.h"
#include "Enemy.h"
#include "P6_UserInterfacesGameState.h"
#include "ProjectileActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShootingDelay = 5.0f;
	bCanShoot = true;
	FieldOfView = 45.0f;
	ShootRange = 1000.0f;

	BulletOffset = CreateDefaultSubobject<USceneComponent>("BulletOffset");
	BulletOffset->SetupAttachment(RootComponent);
	
	HealthWidgetComponent =	CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetRelativeLocation(FVector(0, 0, 120));
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Target = GetWorld()->GetFirstPlayerController()->GetCharacter();
	
	if (EnemyHealthUIClass)
	{
		HealthWidgetComponent->SetWidgetClass(EnemyHealthUIClass);
		HealthWidgetComponent->InitWidget();
		EnemyHealthUI = Cast<UEnemyFloatingHealthBar>(HealthWidgetComponent->GetUserWidgetObject());
		if (EnemyHealthUI)
		{
			EnemyHealthUI->Enemy = this;
			EnemyHealthUI->UpdateValues();
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bCanShoot &&  !bIsRagdolling)
	{
		ReloadTimer += DeltaTime;
		if (ReloadTimer >= ShootingDelay)
		{
			bCanShoot = true;
			ReloadTimer = 0.0f;
		}
	}
	
	if (!Target)
		return;
	if (!ProjectileClass)
		return;
	
	if (bCanShoot && FVector::Distance(GetActorLocation(), Target->GetActorLocation()) <= ShootRange)
	{
		FVector Location = GetActorLocation();
		FVector Direction = Target->GetActorLocation() - Location;
		Direction.Normalize();
		FVector Forward = GetActorForwardVector();
  
		float RotationAngleRad = FMath::Acos(Forward.Dot(Direction));
		float RotationAngleDeg = FMath::RadiansToDegrees(RotationAngleRad);
		
		if (RotationAngleDeg <= FieldOfView)
		{
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
			}
		}
	}
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC)
	{
		FVector PlayerLocation =
			PC->PlayerCameraManager->GetCameraLocation();

		FVector Direction =
			PlayerLocation - HealthWidgetComponent->GetComponentLocation();

		FRotator Rotation = Direction.Rotation();

		HealthWidgetComponent->SetWorldRotation(Rotation);
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Ragdoll()
{
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AEnemy::StopRagdoll, RagdollTime, false);
	bIsRagdolling = true;
}

void AEnemy::StopRagdoll()
{
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

	if (EnemyHealthUI)
	{
		EnemyHealthUI->UpdateValues();
	}

	if (CurrentHealth <= 0)
	{
		AP6_UserInterfacesGameState* GameState = Cast<AP6_UserInterfacesGameState>(GetWorld()->GetGameState());
		if (GameState)
		{
			GameState->Score(true);
		}
		Destroy();
	}
}