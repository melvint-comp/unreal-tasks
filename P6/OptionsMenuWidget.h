// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Types/SlateEnums.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "OptionsMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API UOptionsMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void CloseMenu();
	
	UPROPERTY(meta=(BindWidget))
	UComboBoxString* ColourDeficiencyType;
	UPROPERTY(meta=(BindWidget))
	USlider* ColourDeficiencySeveritySlider;
	UPROPERTY(meta=(BindWidget))
	UCheckBox* ColourCorrectionOnCheckBox;
	bool bCorrectionIsOn = false;
	int ColourDeficiencyIndex = 0;
	float ColourDeficiencySeverity = 0;
	
	void UpdateColourDeficiency();
	UFUNCTION()
	void SeverityUpdate(float Value);
	UFUNCTION()
	void ToggleCorrectionOn(bool Value);
	UFUNCTION()
	void DeficiencyTypeUpdate(FString Value, ESelectInfo::Type NewType);
	
	UPROPERTY(meta=(BindWidget))
	USlider* UIScaleSlider;
	UPROPERTY(meta=(BindWidget))
	UScaleBox* UIScaleBox;
	UFUNCTION()
	void ScaleUI(float Scale);
};
