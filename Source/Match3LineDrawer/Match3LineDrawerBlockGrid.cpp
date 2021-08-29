// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerBlockGrid.h"
#include "Match3LineDrawerBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMatch3LineDrawerBlockGrid::AMatch3LineDrawerBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(600.f, -200.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);
}


void AMatch3LineDrawerBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = SizeHorizontal * SizeVertical;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex / SizeHorizontal) * BlockSpacingHorizontal + (BlockIndex % SizeHorizontal % 2) * BlockSpacingHorizontal / 2.0f; // Divide by dimension
		const float YOffset = (BlockIndex % SizeHorizontal) * BlockSpacingVertical; // Modulo gives remainder
		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation() + GridOffset;

		// Spawn a block
		AMatch3LineDrawerBlock* BlockToAdd = GetWorld()->SpawnActor<AMatch3LineDrawerBlock>(BlockLocation, FRotator(0, 0, 0));
		BlockToAdd->SelectRandomMaterial();

		// Tell the block about its owner
		if (BlockToAdd != nullptr)
		{
			BlockToAdd->OwningGrid = this;
		}
	}
}


void AMatch3LineDrawerBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
