// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boid.h"
#include "Harvester.generated.h"

/**
 * 
 */
UCLASS()
class A3BASECODE_API AHarvester : public ABoid
{
	GENERATED_BODY()

public:

	/*
	 * C2 TASKS IS TO WRITE THESE FUNCTIONS
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
	virtual FVector AvoidBoids(TArray<AActor*> Flock) override;
	virtual FVector VelocityMatching(TArray<AActor*> Flock) override;
	virtual FVector FlockCentering(TArray<AActor*> Flock) override;
	virtual FVector AvoidObstacle() override;
	virtual FVector MoveToTreasure(TArray<AActor*> Flock) override;
	
	virtual void OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnHitboxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) override;
	
	
};
