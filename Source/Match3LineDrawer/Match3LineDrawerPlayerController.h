// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Match3LineDrawerPlayerController.generated.h"

/** PlayerController class used to enable cursor */
UCLASS()
class AMatch3LineDrawerPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMatch3LineDrawerPlayerController();
	void EnableOverEvents(bool ShouldBeActive);
	void EnablePointEvents(bool ShouldBeActive);
	void SetupMouseCursor();
};


