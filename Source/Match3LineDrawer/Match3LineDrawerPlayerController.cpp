// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerPlayerController.h"

AMatch3LineDrawerPlayerController::AMatch3LineDrawerPlayerController()
{
	EnableOverEvents(true);
	EnablePointEvents(true);
	SetupMouseCursor();
}

void AMatch3LineDrawerPlayerController::EnableOverEvents(bool ShouldBeActive)
{
	bEnableMouseOverEvents = ShouldBeActive;
	bEnableTouchOverEvents = ShouldBeActive;
}

void AMatch3LineDrawerPlayerController::EnablePointEvents(bool ShouldBeActive)
{
	bEnableClickEvents = ShouldBeActive;
	bEnableTouchEvents = ShouldBeActive;
}

void AMatch3LineDrawerPlayerController::SetupMouseCursor()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
