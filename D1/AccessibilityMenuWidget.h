// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedActionKeyMapping.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "AccessibilityMenuWidget.generated.h"

UCLASS()
class D1_DFORA_API UAccessibilityMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* MoveForwardRebind;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* MoveBackwardRebind;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* MoveLeftRebind;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* MoveRightRebind;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* JumpRebind;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInputAction* IA_Jump;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void RebindMoveForward(FInputChord SelectedKey);

	UFUNCTION()
	void RebindMoveBackward(FInputChord SelectedKey);

	UFUNCTION()
	void RebindMoveLeft(FInputChord SelectedKey);

	UFUNCTION()
	void RebindMoveRight(FInputChord SelectedKey);

	UFUNCTION()
	void RebindJump(FInputChord SelectedKey);

	UFUNCTION()
	void CloseMenu();

	void RebindKey(
		UInputAction* InputAction,
		const FKey OldKey,
		const FKey NewKey
	);
};



inline void UAccessibilityMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MoveForwardRebind)
	{
		MoveForwardRebind->OnKeySelected.AddDynamic(
			this,
			&UAccessibilityMenuWidget::RebindMoveForward
		);
	}

	if (MoveBackwardRebind)
	{
		MoveBackwardRebind->OnKeySelected.AddDynamic(
			this,
			&UAccessibilityMenuWidget::RebindMoveBackward
		);
	}

	if (MoveLeftRebind)
	{
		MoveLeftRebind->OnKeySelected.AddDynamic(
			this,
			&UAccessibilityMenuWidget::RebindMoveLeft
		);
	}

	if (MoveRightRebind)
	{
		MoveRightRebind->OnKeySelected.AddDynamic(
			this,
			&UAccessibilityMenuWidget::RebindMoveRight
		);
	}

	if (JumpRebind)
	{
		JumpRebind->OnKeySelected.AddDynamic(
			this,
			&UAccessibilityMenuWidget::RebindJump
		);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(
			this,
			&UAccessibilityMenuWidget::CloseMenu
		);
	}
}



inline void UAccessibilityMenuWidget::RebindKey(
	UInputAction* InputAction,
	const FKey OldKey,
	const FKey NewKey
)
{
	if (!IMC_Default || !InputAction)
	{
		return;
	}

	TArray<FEnhancedActionKeyMapping>& Mappings =
		IMC_Default->GetMappings();

	for (FEnhancedActionKeyMapping& Mapping : Mappings)
	{
		if (
			Mapping.Action == InputAction &&
			Mapping.Key == OldKey
		)
		{
			Mapping.Key = NewKey;
			return;
		}
	}
}



inline void UAccessibilityMenuWidget::RebindMoveForward(
	FInputChord SelectedKey
)
{
	if (!IMC_Default || !IA_Move)
	{
		return;
	}

	FKey CurrentKey;

	for (const FEnhancedActionKeyMapping& Mapping :
		IMC_Default->GetMappings())
	{
		if (
			Mapping.Action == IA_Move &&
			Mapping.Key == EKeys::W
		)
		{
			CurrentKey = Mapping.Key;
			break;
		}
	}

	RebindKey(
		IA_Move,
		CurrentKey,
		SelectedKey.Key
	);
}



inline void UAccessibilityMenuWidget::RebindMoveBackward(
	FInputChord SelectedKey
)
{
	if (!IMC_Default || !IA_Move)
	{
		return;
	}

	FKey CurrentKey;

	for (const FEnhancedActionKeyMapping& Mapping :
		IMC_Default->GetMappings())
	{
		if (
			Mapping.Action == IA_Move &&
			Mapping.Key == EKeys::S
		)
		{
			CurrentKey = Mapping.Key;
			break;
		}
	}

	RebindKey(
		IA_Move,
		CurrentKey,
		SelectedKey.Key
	);
}



inline void UAccessibilityMenuWidget::RebindMoveLeft(
	FInputChord SelectedKey
)
{
	if (!IMC_Default || !IA_Move)
	{
		return;
	}

	FKey CurrentKey;

	for (const FEnhancedActionKeyMapping& Mapping :
		IMC_Default->GetMappings()) {
		if (Mapping.Action == IA_Move && Mapping.Key == EKeys::A ) {
			CurrentKey = Mapping.Key;
			break;
		}
	}

	RebindKey(IA_Move,CurrentKey,SelectedKey.Key);
}



inline void UAccessibilityMenuWidget::RebindMoveRight(FInputChord SelectedKey)
{
	if (!IMC_Default || !IA_Move) {
		return;
	}

	FKey CurrentKey;
	for (const FEnhancedActionKeyMapping& Mapping :
		IMC_Default->GetMappings()) {
		if (Mapping.Action == IA_Move && Mapping.Key == EKeys::D) {
			CurrentKey = Mapping.Key;
			break;
		}
	}

	RebindKey(IA_Move,CurrentKey,SelectedKey.Key);
}



inline void UAccessibilityMenuWidget::RebindJump(FInputChord SelectedKey) {
	if (!IMC_Default || !IA_Jump) {
		return;
	}

	FKey CurrentKey;
	for (const FEnhancedActionKeyMapping& Mapping :
		IMC_Default->GetMappings()) {
		if (Mapping.Action == IA_Jump) {
			CurrentKey = Mapping.Key;
			break;
		}
	}

	RebindKey(IA_Jump,CurrentKey,SelectedKey.Key);
}



inline void UAccessibilityMenuWidget::CloseMenu()
{
	RemoveFromParent();

	if (GetWorld()) {
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC) {
			PC->SetInputMode(FInputModeGameOnly());
			PC->SetShowMouseCursor(false);
		}
	}
}