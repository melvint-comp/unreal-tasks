// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EnemyFloatingHealthBar.generated.h"

class UProgressBar;
class AP6_UserInterfacesEnemy;
class AEnemy;

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API UEnemyFloatingHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentHealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthText;

	UPROPERTY()
	AEnemy* Enemy;

	UFUNCTION()
	void UpdateValues();
};