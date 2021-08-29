// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerGameMode.h"
#include "Match3LineDrawerPlayerController.h"
#include "Match3LineDrawerPawn.h"

AMatch3LineDrawerGameMode::AMatch3LineDrawerGameMode()
{
	// no pawn by default
	DefaultPawnClass = AMatch3LineDrawerPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMatch3LineDrawerPlayerController::StaticClass();
}
