// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P6_UserInterfacesGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class AP6_UserInterfacesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	AP6_UserInterfacesGameMode();
	
	virtual void BeginPlay() override;
};



