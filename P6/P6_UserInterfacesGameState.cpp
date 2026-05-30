// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameUI.h"
#include "P6_UserInterfacesCharacter.h"
#include "P6_UserInterfacesGameState.h"

void AP6_UserInterfacesGameState::Score(bool bIsPlayerScore)
{
	if (bIsPlayerScore)
	{
		PlayerScore += 1;
	}
	else
	{
		EnemyScore += 1;
	}
	if (Cast<AP6_UserInterfacesCharacter>(GetWorld()
	 ->GetFirstPlayerController()->GetPawn())->InGameUI)
	{
		Cast<AP6_UserInterfacesCharacter>(GetWorld()
		->GetFirstPlayerController()->GetPawn())
		->InGameUI->UpdateValues();
	}

}
