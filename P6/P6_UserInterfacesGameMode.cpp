// Copyright Epic Games, Inc. All Rights Reserved.

#include "P6_UserInterfacesGameMode.h"

AP6_UserInterfacesGameMode::AP6_UserInterfacesGameMode()
{
	// stub
}

void AP6_UserInterfacesGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
