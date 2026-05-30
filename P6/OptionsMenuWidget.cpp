// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuWidget.h"
#include "P6_UserInterfacesGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UOptionsMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionsMenuWidget::CloseMenu);
	}
	
	if (ColourCorrectionOnCheckBox)
    {
    
    ColourCorrectionOnCheckBox->OnCheckStateChanged.AddDynamic(this,
    &UOptionsMenuWidget::ToggleCorrectionOn);
    }
    if (ColourDeficiencySeveritySlider)
    {
     ColourDeficiencySeveritySlider->OnValueChanged.AddDynamic(this,
    &UOptionsMenuWidget::SeverityUpdate);
    }
	
	if (ColourDeficiencyType)
	{
		ColourDeficiencyType->ClearOptions();

		ColourDeficiencyType->AddOption(TEXT("Normal Vision"));
		ColourDeficiencyType->AddOption(TEXT("Deuteranope"));
		ColourDeficiencyType->AddOption(TEXT("Protanope"));
		ColourDeficiencyType->AddOption(TEXT("Tritanope"));

		ColourDeficiencyType->SetSelectedIndex(0);

		ColourDeficiencyType->OnSelectionChanged.AddDynamic(this,
	   &UOptionsMenuWidget::DeficiencyTypeUpdate);
	}

	if (UIScaleSlider)
	{
		UIScaleSlider->OnValueChanged.AddDynamic(this,
	   &UOptionsMenuWidget::ScaleUI);
	}
}

void UOptionsMenuWidget::CloseMenu()
{
	RemoveFromParent();
}

void UOptionsMenuWidget::UpdateColourDeficiency()
{
	EColorVisionDeficiency VisionDeficiency;
	switch (ColourDeficiencyIndex)
	{
	case 0 :
		VisionDeficiency = EColorVisionDeficiency::NormalVision;
		break;
	case 1 :
		VisionDeficiency = EColorVisionDeficiency::Deuteranope;
		break;
	case 2 :
		VisionDeficiency = EColorVisionDeficiency::Protanope;
		break;
	case 3 :
		VisionDeficiency = EColorVisionDeficiency::Tritanope;
		break;
	default:
		VisionDeficiency = EColorVisionDeficiency::NormalVision;
		break;
	}
	UWidgetBlueprintLibrary::SetColorVisionDeficiencyType(VisionDeficiency, ColourDeficiencySeverity,
	 bCorrectionIsOn, true);
}

void UOptionsMenuWidget::SeverityUpdate(float Value)
{
	ColourDeficiencySeverity = Value;
	UpdateColourDeficiency();
}

void UOptionsMenuWidget::ToggleCorrectionOn(bool Value)
{
	bCorrectionIsOn = Value;
	UpdateColourDeficiency();
}

void UOptionsMenuWidget::DeficiencyTypeUpdate(FString Value, ESelectInfo::Type NewType)
{
	ColourDeficiencyIndex = ColourDeficiencyType->GetSelectedIndex();
	UpdateColourDeficiency();
}

void UOptionsMenuWidget::ScaleUI(float Scale)
{
	if (UIScaleBox)
	{
		UIScaleBox->SetUserSpecifiedScale(Scale);
	}
	
	UP6_UserInterfacesGameInstance* GameInstance = Cast<UP6_UserInterfacesGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetUIScale(UIScaleSlider->GetValue());
	}
}
