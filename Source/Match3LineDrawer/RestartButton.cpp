// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartButton.h"
#include "Match3LineDrawerBlockGrid.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlock"

void ARestartButton::UpdateIndexText()
{
	IndexText->SetRelativeLocation(FVector(-15.0f, -90.f, 50.f));
	IndexText->SetText(LOCTEXT("RestartGame", "Restart"));
}

void ARestartButton::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
}

void ARestartButton::OverBlockEnter(UPrimitiveComponent* ClickedComp)
{
}

void ARestartButton::BlockReleased(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	if (OwningGrid)
	{
		OwningGrid->RestartGame();
	}
}

void ARestartButton::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
}

void ARestartButton::OnFingerEnterBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
}

void ARestartButton::OnFingerReleasedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	if (OwningGrid)
	{
		OwningGrid->RestartGame();
	}
}