// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P6_UserInterfacesGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API UP6_UserInterfacesGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	float UIScaleFactor = 1.0f;

public:
	void SetUIScale(float NewScale);
	float GetUIScale();

};
