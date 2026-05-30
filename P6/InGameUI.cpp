// Fill out your copyright notice in the Description page of Project Settings.


#include "P6_UserInterfacesGameState.h"
#include "InGameUI.h"

void UInGameUI::UpdateValues()
{
	if (Player)
	{
		HealthBar->SetPercent(Player->CurrentHealth / Player->MaxHealth);
		CurrentHealthText->SetText(FText::FromString(FString::FromInt(Player->CurrentHealth)));
		MaxHealthText->SetText(FText::FromString(FString::FromInt(Player->MaxHealth)));
		StaminaBar->SetPercent(Player->CurrentStamina / Player->MaxStamina);
		CurrentStaminaText->SetText(FText::FromString(FString::FromInt(Player->CurrentStamina)));
		MaxStaminaText->SetText(FText::FromString(FString::FromInt(Player->MaxStamina)));
		PunchBar->SetPercent(Player->CurrentPunchTimer/ Player->PunchCooldown);
	}
	
	if (GameState)
	{
		PlayerScoreText->SetText(FText::AsNumber(GameState->PlayerScore));
		EnemyScoreText->SetText(FText::AsNumber(GameState->EnemyScore));
	}
}

void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameState = Cast<AP6_UserInterfacesGameState>(GetWorld()->GetGameState());
	
	// GameInstance = Cast<UP6_UserInterfacesGameInstance>(GetWorld()->GetGameInstance());
	// if (GameInstance)
	// {
	// 	float ScaleFactor = GameInstance->GetUIScale();
	//
	// 	Box1->SetUserSpecifiedScale(ScaleFactor);
	// 	Box2->SetUserSpecifiedScale(ScaleFactor);
	// 	Box3->SetUserSpecifiedScale(ScaleFactor);
	// }
}
