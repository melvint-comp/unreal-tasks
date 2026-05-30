// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitTextUI.generated.h"

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API UHitTextUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HitText;
	UPROPERTY(EditAnywhere)
	TArray<FString> WordList;
	UPROPERTY(EditAnywhere)
	FVector2D TargetLocation = FVector2D(0,-200);
	FVector2D CurrentLocation;
	float CurrentOpacity = 1;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
};
