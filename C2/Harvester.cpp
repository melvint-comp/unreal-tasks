// Fill out your copyright notice in the Description page of Project Settings.


#include "Harvester.h"
#include "Kismet/GameplayStatics.h"
#include "Spawner.h"
#include "Engine/StaticMeshActor.h"

FVector AHarvester::AvoidBoids(TArray<AActor*> Flock)
{
	// To add code here for C2 Simulations
	FVector Steering = FVector::ZeroVector;
	int32 FlockCount = 0;
	FVector AvoidBoidDirection = FVector::ZeroVector;
	float ProximityFactor = 0.0f;
	
	for (AActor* OverlapActor : Flock)
	{
		AHarvester* OtherBoid = Cast<AHarvester>(OverlapActor);
		if (OtherBoid != nullptr || OtherBoid != this)
			continue;

		AvoidBoidDirection = this->GetActorLocation() - OverlapActor->GetActorLocation();
		AvoidBoidDirection = AvoidBoidDirection.GetSafeNormal();
		ProximityFactor = 1.0f - (AvoidBoidDirection.Size() / this->PerceptionSensor->GetScaledSphereRadius());

		Steering += (ProximityFactor * AvoidBoidDirection);
		FlockCount++;
	}
	
	if (FlockCount > 0)
	{
		Steering /= FlockCount;
		Steering.GetSafeNormal() -= this->BoidVelocity.GetSafeNormal();
		Steering *= AvoidBoidsStrength;
		return Steering;
	}
	
	return FVector::ZeroVector;
}

FVector AHarvester::VelocityMatching(TArray<AActor*> Flock)
{
	// To add code here for C2 Simulations
	FVector Steering = FVector::ZeroVector;
	int32 FlockCount = 0.0f;
	
	for (AActor* OverlapActor : Flock)
	{
		AHarvester* OtherBoid = Cast<AHarvester>(OverlapActor);
		if (OtherBoid == nullptr || OtherBoid == this)
			continue;

		Steering += OtherBoid->BoidVelocity.GetSafeNormal();
		FlockCount++;
	}
	
	if (FlockCount > 0)
	{
		//get alignment force to average flock direction
		Steering /= FlockCount;
		Steering.GetSafeNormal() -= this->BoidVelocity.GetSafeNormal();
		Steering *= VelocityMatchingStrength;
		return Steering;
	}
	
	return FVector::ZeroVector;
}

FVector AHarvester::FlockCentering(TArray<AActor*> Flock)
{
	// To add code here for C2 Simulations
	FVector Steering = FVector::ZeroVector;
	int32 FlockCount = 0.0f;
	FVector AveragePosition = FVector::ZeroVector;
	
	for (AActor* OverlapActor : Flock)
	{
		AHarvester* OtherBoid = Cast<AHarvester>(OverlapActor);
		if (OtherBoid == nullptr || OtherBoid == this)
			continue;

		AveragePosition += OtherBoid->GetActorLocation();
		FlockCount++;
	}
	
	if (FlockCount > 0)
	{
		//average cohesion force of flock
		AveragePosition /= FlockCount;
		Steering = AveragePosition - this->GetActorLocation();
		Steering.GetSafeNormal() -= this->BoidVelocity.GetSafeNormal();
		Steering *= CenteringStrength;
		return Steering;
	}
	
	return FVector::ZeroVector;
}

FVector AHarvester::AvoidObstacle()
{
	// To add code here for C2 Simulations
	FVector Steering = FVector::ZeroVector;
	FQuat SensorRotation = FQuat::FindBetweenVectors(AvoidanceSensors[0],
	this->GetActorForwardVector());
	FVector NewSensorDirection = FVector::ZeroVector;
	FCollisionQueryParams TraceParameters;
	FHitResult Hit;
	
	for (FVector AvoidanceSensor : AvoidanceSensors)
	{
		NewSensorDirection = SensorRotation.RotateVector(AvoidanceSensor);
		GetWorld()->LineTraceSingleByChannel(Hit,
		this->GetActorLocation(), this->GetActorLocation() +
		NewSensorDirection * SensorRadius, ECC_GameTraceChannel1,
		TraceParameters);
		
		if (!Hit.bBlockingHit)
		{
			Steering = NewSensorDirection.GetSafeNormal() - this->BoidVelocity.GetSafeNormal();
			Steering *= ObstacleAvoidanceStrength;
			return Steering;
		}
	}
	
	return FVector::ZeroVector;
}

FVector AHarvester::MoveToTreasure(TArray<AActor*> Flock)
{
	FVector Steering = FVector::ZeroVector;

	TArray<AActor*> Asteroids;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AAsteroidCluster::StaticClass(),
		Asteroids
	);

	AAsteroidCluster* ClosestAsteroid = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	for (AActor* Actor : Asteroids)
	{
		AAsteroidCluster* Asteroid = Cast<AAsteroidCluster>(Actor);

		if (Asteroid == nullptr)
			continue;

		float Distance = FVector::Dist(
			GetActorLocation(),
			Asteroid->GetActorLocation()
		);

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestAsteroid = Asteroid;
		}
	}

	if (ClosestAsteroid != nullptr)
	{
		Steering = ClosestAsteroid->GetActorLocation() - GetActorLocation();
		Steering = Steering.GetSafeNormal();
		Steering -= BoidVelocity.GetSafeNormal();
		Steering *= AsteroidStrength;
		return Steering;
	}

	return FVector::ZeroVector;
}

// Check collisions with other objects, specifically for Asteroids (point gathering) and Walls (death)
void AHarvester::OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Invincibility < 0)
	{
		if(OtherActor && OtherActor != this && OverlappedComponent->GetName().Equals(TEXT("Boid Collision Component")))
		{
			AAsteroidCluster* Asteroids = Cast<AAsteroidCluster>(OtherActor);
			if(Asteroids)
			{
				CollisionAsteroids = Asteroids;
				return;
			}

			AStaticMeshActor* Wall = Cast<AStaticMeshActor>(OtherActor);
			if(Wall)
			{
				Spawner->NumHarvesterShips--;
				Destroy();
				return;
			}
		}
	}
}

// If we leave an asteroid field update so that we know not to try and extract resources
void AHarvester::OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AAsteroidCluster* Asteroids = Cast<AAsteroidCluster>(OtherActor);
	if(Asteroids)
	{
		CollisionAsteroids = nullptr;
	}
}
