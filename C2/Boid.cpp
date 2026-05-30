// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"

#include "Harvester.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABoid::ABoid()
{
	PrimaryActorTick.bCanEverTick = true;

	BoidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boid Mesh Component"));
	BoidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoidMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	BoidCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Boid Collision Component"));
	BoidCollision->SetCollisionObjectType(ECC_Pawn);
	BoidCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoidCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoidCollision->SetSphereRadius(100.0f);

	RootComponent = BoidMesh;
	BoidCollision->SetupAttachment(RootComponent);

	BoidCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoid::OnHitboxOverlapBegin);
	BoidCollision->OnComponentEndOverlap.AddDynamic(this, &ABoid::OnHitboxOverlapEnd);
	
	PerceptionSensor = CreateDefaultSubobject<USphereComponent>(TEXT("Perception Sensor Component"));
	PerceptionSensor->SetupAttachment(RootComponent);
	PerceptionSensor->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PerceptionSensor->SetCollisionResponseToAllChannels(ECR_Ignore);
	PerceptionSensor->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	PerceptionSensor->SetSphereRadius(1000.0f);
	
	BoidVelocity = FVector::ZeroVector;
	
	AvoidanceSensors.Empty();
	
	FVector SensorDirection;

	// This is used to calculate points on a sphere for sensing. These are not implemented currently
	for (int i = 0; i < NumSensors; ++i)
	{
		float Yaw = 2 * UKismetMathLibrary::GetPI() * GoldenRatio * i;
		float Pitch = FMath::Acos(1 - (2 * float(i) / NumSensors));
		
		SensorDirection.X = FMath::Cos(Yaw)*FMath::Sin(Pitch);
		SensorDirection.Y = FMath::Sin(Yaw)*FMath::Sin(Pitch);
		SensorDirection.Z = FMath::Cos(Pitch);

		AvoidanceSensors.Emplace(SensorDirection);
	}
}

void ABoid::BeginPlay()
{
	Super::BeginPlay();

	FRotator SpawnRotation(FMath::RandRange(-180,180),FMath::RandRange(-180,180),FMath::RandRange(-180,180));
	SetActorRotation(SpawnRotation);
	CurrentRotation = GetActorRotation();

	BoidVelocity = GetActorForwardVector();
	BoidVelocity*= FMath::RandRange(MinSpeed, MaxSpeed);
}

// Simple tick function that delegates to other functions for various needs. Calls the FlightPath which calculates the
// boid movement
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FlightPath(DeltaTime);
	UpdateMeshRotation();

	if(CollisionAsteroids) {
		ResourcesCollected += CollisionAsteroids->RemoveGold();
	}

	if(Invincibility > 0){
		Invincibility-= DeltaTime;
	}
}

// Updates the mesh rotation using interpolation so there is a smooth transition from current facing to new rotation
void ABoid::UpdateMeshRotation()
{
	CurrentRotation = FMath::RInterpTo(CurrentRotation, GetActorRotation(), GetWorld()->DeltaTimeSeconds, 7.0f);
	SetActorRotation(CurrentRotation);
}

/*
 *
 * NOT IMPLEMENTED IN THIS PROJECT
 * These functions are virtual and are overridden by the child classes (in this case Harvester)
 * No logic should be added here
 *
 */
FVector ABoid::AvoidBoids(TArray<AActor*> Flock)
{
	return FVector::ZeroVector;
}

/*
 *
 * NOT IMPLEMENTED IN THIS PROJECT
 * These functions are virtual and are overridden by the child classes (in this case Harvester)
 * No logic should be added here
 *
 */
FVector ABoid::VelocityMatching(TArray<AActor*> Flock)
{
	return FVector::ZeroVector;
}

/*
 *
 * NOT IMPLEMENTED IN THIS PROJECT
 * These functions are virtual and are overridden by the child classes (in this case Harvester)
 * No logic should be added here
 *
 */
FVector ABoid::FlockCentering(TArray<AActor*> Flock)
{
	return FVector::ZeroVector;
}

/*
 *
 * NOT IMPLEMENTED IN THIS PROJECT
 * These functions are virtual and are overridden by the child classes (in this case Harvester)
 * No logic should be added here
 *
 */
FVector ABoid::AvoidObstacle()
{
	return FVector::ZeroVector;
}

// This function is used to calculate if there is an obstacle ahead (i.e. a wall)
// This is a simple implementation and could be adjusted for more advanced approaches to avoidance
bool ABoid::IsObstacleAhead()
{
	if (AvoidanceSensors.Num() > 0)
	{
		FCollisionQueryParams TraceParameters;
		FHitResult Hit;
		
		GetWorld()->LineTraceSingleByChannel(Hit,this->GetActorLocation(),this->GetActorLocation() + GetActorForwardVector() * SensorRadius,ECC_GameTraceChannel1,TraceParameters);

		if (Hit.bBlockingHit)
		{
			TArray<AActor*> OverlapActors;
			BoidCollision->GetOverlappingActors(OverlapActors);
			for (AActor* OverlapActor : OverlapActors)
			{
				if (Hit.GetActor() == OverlapActor)
				{
					return false;
				}
			}
		}
		return Hit.bBlockingHit;
	}
	return false;
}

/* 
 * Implemented in child class
 */
void ABoid::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

/* 
 * Implemented in child class
 */
void ABoid::OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	
}

/*
 *
 * C2 IMPLEMENTATION GOES HERE
 * This function is used to calculate the current flight path of the boids
 * You will be filling in this function as part of the C2 tasks
 *
 */
void ABoid::FlightPath(float DeltaTime)
{
	// To add code here for C2 Simulations
	FVector Acceleration = FVector::ZeroVector;
	
	this->SetActorLocation(this->GetActorLocation() + (BoidVelocity * DeltaTime));
	this->SetActorRotation(BoidVelocity.ToOrientationQuat());
	
	TArray<AActor*> OverlapActors;
	PerceptionSensor->GetOverlappingActors(OverlapActors);
	
	Acceleration += AvoidBoids(OverlapActors);
	Acceleration += VelocityMatching(OverlapActors);
	Acceleration += FlockCentering(OverlapActors);
	Acceleration += MoveToTreasure(OverlapActors);
	
	if (IsObstacleAhead())
	{
		Acceleration += AvoidObstacle();
	}
	
	BoidVelocity += (Acceleration * DeltaTime);
	BoidVelocity = BoidVelocity.GetClampedToSize(MinSpeed, MaxSpeed);
}

FVector ABoid::MoveToTreasure(TArray<AActor*> NearbyActors)
{
	return FVector::ZeroVector;
}
