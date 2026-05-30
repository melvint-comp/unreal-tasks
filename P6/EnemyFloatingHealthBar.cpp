// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Enemy.h"
#include "EnemyFloatingHealthBar.h"

void UEnemyFloatingHealthBar::UpdateValues()
{
	if (Enemy)
	{
		HealthBar->SetPercent(Enemy->CurrentHealth / Enemy->MaxHealth);
		CurrentHealthText->SetText(FText::FromString(FString::FromInt(Enemy->CurrentHealth)));
		MaxHealthText->SetText(FText::FromString(FString::FromInt(Enemy->MaxHealth)));
	}
}