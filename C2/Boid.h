// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsteroidCluster.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

// Forward declaration of the Spawner Actor.
// The boids need to know about the spawner and vise-versa
class ASpawner;

UCLASS()
class A3BASECODE_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoid();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Static Mesh for the boid
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BoidMesh;
	
	// This is the closer collision boundary for the boid. If this is triggered something is directly touching the boid
	UPROPERTY(EditAnywhere)
		USphereComponent* BoidCollision;
	
	// This volume is used to percieve the world around it. Consider this like the boid's vision range.
	UPROPERTY(EditAnywhere)
		USphereComponent* PerceptionSensor;

	// Not implemented in the base code
	// Boids can collect resources from asteroid clusters
	// This could be a flocking target
	UPROPERTY(VisibleAnywhere)
		float ResourcesCollected = 0.0f;

	// Which direction and speed is the boid moving in
	FVector BoidVelocity;

	// We track current rotation and get boids to update each frame
	// This allows us to smoothly rotate the mesh using LERP instead of instant change
	FRotator CurrentRotation;
	void UpdateMeshRotation();

	/*
	 * C2 TASKS WILL REQUIRE ADJUSTING THESE IN THE HARVESTER
	 * Boids follow simple rules that when run in a system begin to demonstrate flocking behaviour.
	 *
	 * AvoidBoids - Separation
	 * This function ensures that boids do not collide with each other. This applies a force pushing away
	 * from nearby boids to ensure they remain in close proximity but never overlapping
	 *
	 * VelocityMatching - Alignment
	 * This function applies a force that roughly aligns the forward direction and speed of the boids so they align.
	 *
	 * FlockCentering - Cohesion
	 * This acts as a countering force to seperation that tries to keep boids around a shared centrepoint. The stronger
	 * this is then the stronger the boids will try to go to this location.
	 *
	 * AvoidObstacle - Obstacle Avoidance
	 * This acts to push boids away from obstacles, which in this case is our walls. In this simulation boids "die"
	 * when they touch solid objects so this acts as a repulsion away from them. This has the highest priority when
	 * calculating everything together.
	 */
	virtual FVector AvoidBoids(TArray<AActor*> Flock);
	virtual FVector VelocityMatching(TArray<AActor*> Flock);
	virtual FVector FlockCentering(TArray<AActor*> Flock);
	virtual FVector AvoidObstacle();
	virtual FVector MoveToTreasure(TArray<AActor*> Flock);

	// This function combines all of the boid rules together and uses them to calculate the final movement each frame
	virtual void FlightPath(float DeltaTime);

	// This function is used to check if there are walls nearby. This returns true if there is
	// Currently we are doing a very basic check of directly infront the boid
	// We actually have more advanced methods using our avoidance sensors
	bool IsObstacleAhead();

	// This is an array of forces from nearby asteroids. Currently this is not implemented
	TArray<FVector> AsteroidForces;

	// These are the minimum and maximum speeds the boids can move at. This ensures that our boids do not get infinitely faster or slower
	float MinSpeed = 300.0f;
	float MaxSpeed = 600.0f;

	/*
	 * C2 TASKS RELATED TO THESE VALUES
	 * These values here are the multiplying factors that affect the weight of each rule.
	 * As an example because avoiding obstacles is such an important task, the strength here massively outweighs all others.
	 * The provided values are good defaults but do not hesitate to play around with them and observe the effects.
	 */
	float VelocityMatchingStrength = 200.0f;
	float AvoidBoidsStrength = 30.0f;
	float CenteringStrength = 5.0f;
	float ObstacleAvoidanceStrength = 150.0f;
	float AsteroidStrength = 250.0f;

	// When boids first spawn-in they have an invincibility timer of 1 second where they cannot be destroyed
	// We have disabled boid death upon collision with each other so this is less of an issue
	float Invincibility = 1.0f;

	// Not currently implemented in solution for obstacle avoidance
	// These avoidance sensors create a cluster of points that each boid can check in a sphere to see if they are near obstacles.
	// This can be used to work out a safe direction nearest to the wall or other obstacle.
	TArray<FVector> AvoidanceSensors;
	float SensorRadius = 1200.0f;
	int NumSensors = 100;

	// This is used for determining spawn locations of these avoidance sensors above.
	// A good video explaining this in detail can be found here: https://www.youtube.com/watch?v=bqtqltqcQhw
	float GoldenRatio = (1.0f + FMath::Sqrt(5.0f))/2;
	
	// Not implemented in current project
	// If we are currently in an asteroid field it will be stored here
	AAsteroidCluster* CollisionAsteroids;
	
	// A reference to the spawner in charge of spawning in boids
	ASpawner* Spawner;

	UFUNCTION()
		virtual void OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent,  AActor* OtherActor,  UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,  bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent,  AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};