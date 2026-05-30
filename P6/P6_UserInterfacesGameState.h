// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P6_UserInterfacesGameState.generated.h"

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API AP6_UserInterfacesGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	int PlayerScore = 0;
	int EnemyScore = 0;
	void Score(bool bIsPlayerScore);
	
};
