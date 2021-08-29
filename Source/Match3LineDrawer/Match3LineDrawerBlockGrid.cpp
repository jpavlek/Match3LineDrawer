// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerBlockGrid.h"
#include "Match3LineDrawerBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include "Math/IntPoint.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMatch3LineDrawerBlockGrid::AMatch3LineDrawerBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(600.f, -200.f, 0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);
}

void AMatch3LineDrawerBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = SizeHorizontal * SizeVertical;

	// Loop to spawn each block
	for (int32 BlockIndex = 0; BlockIndex < NumBlocks; BlockIndex++)
	{
		const FVector BlockLocation = IndexTo3DCoordinate(BlockIndex);

		// Spawn a block
		AMatch3LineDrawerBlock* BlockToAdd = GetWorld()->SpawnActor<AMatch3LineDrawerBlock>(BlockLocation, FRotator(0, 0, 0));
		BlockToAdd->SelectRandomColor();
		BlockToAdd->UpdateMaterial();
		BlockToAdd->SetIndex(BlockIndex);

		// Tell the block about its owner
		if (BlockToAdd != nullptr)
		{
			BlockToAdd->OwningGrid = this;
		}
		Tiles.Add(BlockIndex, BlockToAdd);
	}
}

int32 AMatch3LineDrawerBlockGrid::GetLastSelectedBlockIndex() const
{
	return LastSelectedBlockIndex;
}

void AMatch3LineDrawerBlockGrid::SetLastSelectedBlockIndex(int32 IndexToSet)
{
	LastSelectedBlockIndex = IndexToSet;
}

AMatch3LineDrawerBlock* AMatch3LineDrawerBlockGrid::GetLastSelectedBlock()
{
	return GetTile(LastSelectedBlockIndex);
}

void AMatch3LineDrawerBlockGrid::EvaluateTilesSelection()
{
	SelectionEnabled = false;
	if (NumberOfSelectedTiles >= 3)
	{
		AddScore();

		// Hide selected tiles.
		ShowSelectedTiles(false);

		// Remove highlight if any, restore original colored materials
		RefreshColors();

		// Simulate tiles falling by swapping selected tiles with any tiles on top of them
		SwapSelectedTiles();

		// Reuse selected tiles with randomized colors
		RandomizeSelectedTilesColors();

		// Show 'selected' tiles on top of the grid.
		ShowSelectedTiles(true);
	}

	// Deselect selected tiles in any case
	DeselectAllTiles();
}

int32 AMatch3LineDrawerBlockGrid::CalculateScoreIncrease()
{
	int32 scoreIncrease = NumberOfSelectedTiles * NumberOfSelectedTiles * 10;
	return scoreIncrease;
}

void AMatch3LineDrawerBlockGrid::AddScore()
{
	int32 amount = CalculateScoreIncrease();
	AddScore(amount);
}

void AMatch3LineDrawerBlockGrid::AddScore(int32 Amount)
{
	Score += Amount;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

FIntPoint AMatch3LineDrawerBlockGrid::IndexToGridCoordinate(int32 index) const
{
	int32 row = index / SizeHorizontal;
	int32 column = index % SizeHorizontal;
	return FIntPoint(row, column);
}

FVector2D AMatch3LineDrawerBlockGrid::GridCoordinateToOffset(const FIntPoint& position) const
{
	const FVector2D result(position.X * BlockSpacingHorizontal + (position.Y % 2) * BlockSpacingHorizontal / 2.0f, // Divide by dimension
		position.Y * BlockSpacingVertical); // Modulo gives remainder
	return result;
}

FVector AMatch3LineDrawerBlockGrid::IndexToCoordinate(int32 index) const
{
	int32 row = index / SizeHorizontal;
	int32 column = index % SizeHorizontal;
	return FVector(row, column, 0.0f);
}

int32 AMatch3LineDrawerBlockGrid::GridCoordinateToIndex(const FIntPoint& Position) const
{
	int32 index = Position.X * SizeHorizontal + Position.Y;
	return index;
}

FVector AMatch3LineDrawerBlockGrid::OffsetTo3DCoordinate(const FVector2D& RelativeOffset) const
{
	return GetActorLocation() + GridOffset + FVector(RelativeOffset.X, RelativeOffset.Y, 0.f);
}

FVector AMatch3LineDrawerBlockGrid::IndexTo3DCoordinate(int32 BlockIndex) const
{
	const FIntPoint position = IndexToGridCoordinate(BlockIndex);
	const FVector2D relativeOffset = GridCoordinateToOffset(position);
	const FVector BlockLocation = OffsetTo3DCoordinate(relativeOffset);
	return BlockLocation;
}

int32 AMatch3LineDrawerBlockGrid::GetNumberOfSelectedTiles()
{
	return NumberOfSelectedTiles;
}

AMatch3LineDrawerBlock* AMatch3LineDrawerBlockGrid::GetTile(int32 Index)
{
	if (Index == -1)
	{
		return nullptr;
	}

	return Tiles[Index];
}

void AMatch3LineDrawerBlockGrid::DeselectBlockOnReturn()
{
	AMatch3LineDrawerBlock* lastSelectedBlock = GetLastSelectedBlock();
	LastSelectedBlockIndex = lastSelectedBlock->GetLastSelectedBlockIndex();
	lastSelectedBlock->DeselectBlock();
}

void AMatch3LineDrawerBlockGrid::DeselectAllTiles()
{
	TArray<int32> indices;
	SelectedTiles.GetKeys(indices);
	for (int32 index : indices)
	{
		if (AMatch3LineDrawerBlock* block = GetTile(index))
		{
			block->DeselectBlock();
		}
	}
	SelectedTiles.Empty();
	SelectedTiles.Compact();
	LastSelectedBlockIndex = -1;

}

void AMatch3LineDrawerBlockGrid::ShowSelectedTiles(bool ShouldShow)
{
	TArray<int32> indices;
	SelectedTiles.GetKeys(indices);
	for (int32 index : indices)
	{
		if (AMatch3LineDrawerBlock* block = GetTile(index))
		{
			block->SetActorHiddenInGame(!ShouldShow);
		}
	}
}

void AMatch3LineDrawerBlockGrid::RefreshColors()
{
	TArray<int32> indices;
	SelectedTiles.GetKeys(indices);
	for (int32 index : indices)
	{
		if (AMatch3LineDrawerBlock* block = GetTile(index))
		{
			block->UpdateMaterial();
		}
	}
}

void AMatch3LineDrawerBlockGrid::RandomizeSelectedTilesColors()
{
	TArray<int32> indices;
	SelectedTiles.GetKeys(indices);
	for (int32 index : indices)
	{
		if (AMatch3LineDrawerBlock* block = GetTile(index))
		{
			block->SelectRandomColor();
			block->UpdateMaterial();
		}
	}
}

void AMatch3LineDrawerBlockGrid::AddSelectedTile(int32 IndexToAdd)
{
	NumberOfSelectedTiles++;
	SelectedTiles.Add(IndexToAdd, IndexToAdd);
}

void AMatch3LineDrawerBlockGrid::RemoveSelectedTile(int32 IndexToRemove)
{
	NumberOfSelectedTiles--;
	SelectedTiles.Remove(IndexToRemove);
}

bool AMatch3LineDrawerBlockGrid::SwapTiles(int32 TileIndex0, int32 TileIndex1)
{
	TArray<int32> selectedIndices;
	SelectedTiles.GetKeys(selectedIndices);
	bool isSelectedIndex0 = selectedIndices.Contains(TileIndex0);
	bool isSelectedIndex1 = selectedIndices.Contains(TileIndex1);
	if (isSelectedIndex0 == isSelectedIndex1)
	{
		return false;
	}

	int32 selectedIndex = TileIndex0;
	int32 otherIndex = TileIndex1;
	if (isSelectedIndex1)
	{
		selectedIndex = TileIndex1;
		otherIndex = TileIndex0;
	}

	AMatch3LineDrawerBlock* selectedBlock = GetTile(selectedIndex);
	AMatch3LineDrawerBlock* otherBlock = GetTile(otherIndex);

	selectedBlock->SetIndex(otherIndex);
	otherBlock->SetIndex(selectedIndex);

	selectedBlock->SetActorLocation(IndexTo3DCoordinate(otherIndex));
	otherBlock->SetActorLocation(IndexTo3DCoordinate(selectedIndex));

	Tiles[selectedIndex] = otherBlock;
	Tiles[otherIndex] = selectedBlock;

	SelectedTiles.Remove(selectedIndex);
	SelectedTiles.Add(otherIndex);
	return true;
}

void AMatch3LineDrawerBlockGrid::SwapSelectedTiles()
{
	int32 TotalSize = SizeHorizontal * SizeVertical;
	bool swapped = false;
	do
	{
		TArray<int32> indices;
		SelectedTiles.GetKeys(indices);
		indices.Sort([](const int32 a, const int32 b) { return a > b; });
		swapped = false;
		for (int32 index : indices)
		{
			int32 otherIndex = index + SizeHorizontal;
			if (otherIndex >= TotalSize)
			{
				continue;
			}
			swapped = swapped || SwapTiles(index, index + SizeHorizontal);
		}

	} while (swapped);
}

#undef LOCTEXT_NAMESPACE
