// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "P6_UserInterfacesGameState.h"
#include "CoreMinimal.h"
#include "P6_UserInterfacesCharacter.h"
#include "P6_UserInterfacesGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ScaleBox.h"
#include "Components/TextBlock.h"
#include "InGameUI.generated.h"

/**
 * 
 */

class AP6_UserInterfacesGameState;

UCLASS()
class P6_USERINTERFACES_API UInGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaxHealthText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentHealthText;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaBar;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MaxStaminaText;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CurrentStaminaText;
    UPROPERTY(meta = (BindWidget))
    UProgressBar* PunchBar;
    
    UPROPERTY()
    AP6_UserInterfacesCharacter* Player;
    
    void UpdateValues();
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerScoreText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* EnemyScoreText;
	UPROPERTY()
	AP6_UserInterfacesGameState* GameState;
	virtual void NativeConstruct() override;
	
	// UPROPERTY(meta = (BindWidget))
	// UScaleBox* Box1;
	// UPROPERTY(meta = (BindWidget))
	// UScaleBox* Box2;
	// UPROPERTY(meta = (BindWidget))
	// UScaleBox* Box3;
	// UP6_UserInterfacesGameInstance* GameInstance;
};
