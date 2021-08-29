// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3LineDrawerBlock.h"
#include "Match3LineDrawerBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AMatch3LineDrawerBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	AMatch3LineDrawerBlockGrid();

	int32 Score;

	/** Index of last selected block */
	int32 LastSelectedBlockIndex = -1;
	
	/** Number of currently selected block tiles */
	int32 NumberOfSelectedTiles = 0;
	
	/** Number of blocks along horizontal side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeHorizontal = 7;

	/** Number of blocks along vertical side of grid */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeVertical = 6;
	
	/** Horizontal spacing between blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacingHorizontal = 300.0f;
	
	/** Vertical spacing between blocks */
	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacingVertical = 220.0f;

	/** Offset of the whole grid relative to the screen */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	FVector GridOffset = FVector(300.0f, 300, 0.0f);

	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly)
	bool SelectionEnabled = false;

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	/** Handle the block being clicked */
	void AddScore();

	void AddScore(int32 Amount);

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

	/** Calculates grid coordinates (row, column) from index **/
	FIntPoint IndexToGridCoordinate(int32 index) const;

	FVector2D GridCoordinateToOffset(const FIntPoint& position) const;

	FVector IndexToCoordinate(int32 index) const;

	int32 GridCoordinateToIndex(const FIntPoint& Position) const;

	FVector OffsetTo3DCoordinate(const FVector2D& RelativeOffset) const;

	FVector IndexTo3DCoordinate(int32 BlockIndex) const;

	int32 GetNumberOfSelectedTiles();

	AMatch3LineDrawerBlock* GetTile(int32 Index);

	void DeselectAllTiles();

	void ShowSelectedTiles(bool ShouldShow = true);

	void RefreshColors();

	void RandomizeSelectedTiles();

	bool SwapTiles(int32 TileIndex0, int32 TileIndex1);

	void SwapSelectedTiles();

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	TMap<int32, AMatch3LineDrawerBlock*> Tiles;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
	TMap<int32, int32> SelectedTiles;

	void HideBlock();
};



