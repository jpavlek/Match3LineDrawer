// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Match3LineDrawerBlock.h"
#include "RestartButton.generated.h"

/**
 *
 */
UCLASS()
class MATCH3LINEDRAWER_API ARestartButton : public AMatch3LineDrawerBlock
{
	GENERATED_BODY()

public:
	/** Handle the block being clicked */
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked) override;

	void UpdateIndexText();

	void OverBlockEnter(UPrimitiveComponent* ClickedComp) override;

	void BlockReleased(UPrimitiveComponent* ClickedComp, FKey ButtonClicked) override;

	/** Handle the block being touched  */
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent) override;

	void OnFingerEnterBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent) override;

	void OnFingerReleasedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent) override;
};
