// Fill out your copyright notice in the Description page of Project Settings.


#include "P6_UserInterfacesGameInstance.h"

void UP6_UserInterfacesGameInstance::SetUIScale(float NewScale)
{
	UIScaleFactor = NewScale;
}

float UP6_UserInterfacesGameInstance::GetUIScale()
{
	return UIScaleFactor;
}
