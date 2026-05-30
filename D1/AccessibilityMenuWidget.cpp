// // Fill out your copyright notice in the Description page of Project Settings.
//
// #include "EnhancedInputComponent.h"
// #include "AccessibilityMenuWidget.h"
//
// void UAccessibilityMenuWidget::NativeConstruct()
// {
// 	Super::NativeConstruct();
// 	
// 	GEngine->AddOnScreenDebugMessage(
// 	-1,
// 	5.f,
// 	FColor::Red,
// 	TEXT("NativeConstruct Running")
// );
// 	if (BackButton)
// 	{
// 		GEngine->AddOnScreenDebugMessage(
// 			-1,
// 			5.f,
// 			FColor::Green,
// 			TEXT("BackButton Found")
// 		);
//
// 		BackButton->OnClicked.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::CloseMenu
// 		);
// 	}
//
// 	if (MoveForwardRebind)
// 	{
// 		MoveForwardRebind->OnKeySelected.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::RebindMoveForward
// 		);
// 	}
//
// 	if (MoveBackwardRebind)
// 	{
// 		MoveBackwardRebind->OnKeySelected.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::RebindMoveBackward
// 		);
// 	}
//
// 	if (MoveLeftRebind)
// 	{
// 		MoveLeftRebind->OnKeySelected.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::RebindMoveLeft
// 		);
// 	}
//
// 	if (MoveRightRebind)
// 	{
// 		MoveRightRebind->OnKeySelected.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::RebindMoveRight
// 		);
// 	}
//
// 	if (JumpRebind)
// 	{
// 		JumpRebind->OnKeySelected.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::RebindJump
// 		);
// 	}
//
// 	if (BackButton)
// 	{
// 		BackButton->OnClicked.AddDynamic(
// 			this,
// 			&UAccessibilityMenuWidget::CloseMenu
// 		);
// 	}
// }
//
// void UAccessibilityMenuWidget::RebindMoveForward(FInputChord SelectedKey)
// {
// 	if (!IMC_Default || !IA_Move)
// 	{
// 		return;
// 	}
// 	IMC_Default->UnmapKey(IA_Move, EKeys::W);
// 	IMC_Default->MapKey(IA_Move, SelectedKey.Key);
// }
//
// void UAccessibilityMenuWidget::RebindMoveBackward(FInputChord SelectedKey)
// {
// 	if (!IMC_Default || !IA_Move)
// 	{
// 		return;
// 	}
// 	IMC_Default->UnmapKey(IA_Move, EKeys::S);
// 	IMC_Default->MapKey(IA_Move, SelectedKey.Key);
// }
//
// void UAccessibilityMenuWidget::RebindMoveLeft(FInputChord SelectedKey)
// {
// 	if (!IMC_Default || !IA_Move)
// 	{
// 		return;
// 	}
// 	IMC_Default->UnmapKey(IA_Move, EKeys::A);
// 	IMC_Default->MapKey(IA_Move, SelectedKey.Key);
// }
//
// void UAccessibilityMenuWidget::RebindMoveRight(FInputChord SelectedKey)
// {
// 	if (!IMC_Default || !IA_Move)
// 	{
// 		return;
// 	}
// 	IMC_Default->UnmapKey(IA_Move, EKeys::D);
// 	IMC_Default->MapKey(IA_Move, SelectedKey.Key);
// }
//
// void UAccessibilityMenuWidget::RebindJump(FInputChord SelectedKey)
// {
// 	if (!IMC_Default || !IA_Move)
// 	{
// 		return;
// 	}
// 	IMC_Default->UnmapKey(IA_Move, EKeys::SpaceBar);
// 	IMC_Default->MapKey(IA_Move, SelectedKey.Key);
// }
//
// void UAccessibilityMenuWidget::CloseMenu()
// {
// 	RemoveFromParent();
//
// 	if (GetWorld())
// 	{
// 		APlayerController* PC =
// 			GetWorld()->GetFirstPlayerController();
//
// 		if (PC)
// 		{
// 			PC->SetInputMode(FInputModeGameOnly());
// 			PC->SetShowMouseCursor(false);
// 		}
// 	}
// }
