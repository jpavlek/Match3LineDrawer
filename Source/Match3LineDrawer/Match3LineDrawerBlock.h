// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DA_BlockMeshHex.h"
#include "Match3LineDrawerBlock.generated.h"

UENUM()
enum class ETileColor : uint8
{
	WHITE,
	RED,
	GREEN,
	BLUE,
	ORANGE,
	PURPLE
};

/** A block that can be clicked */
UCLASS(minimalapi)
class AMatch3LineDrawerBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMeshHex;

	/** Index in a grid */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Index = -1;

	/** Index of the last selected grid tile */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 LastSelectedBlockIndex = -1;

	UDA_BlockMeshHex* blockDataAsset = NewObject<UDA_BlockMeshHex>();

protected:
	/** Text component for displaying grid index */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* IndexText;

public:
	AMatch3LineDrawerBlock();

	void SetIndexRelativeLocation(const FVector& RelativeLocationToSet);

	int32 GetLastSelectedBlockIndex() const;

	UFUNCTION()
	UMaterialInstance* SelectMaterial(const ETileColor& Color);

	void UpdateMaterial();

	void SetColor(const ETileColor color);

	ETileColor SelectRandomColor();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to red material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* RedMaterial;

	/** Pointer to green material used on active blocks */
	UPROPERTY()
	UMaterialInstance* GreenMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	UMaterialInstance* OrangeMaterial;

	/** Pointer to purple material used on active blocks */
	UPROPERTY()
	UMaterialInstance* PurpleMaterial;

	/** Pointer to white material used on active blocks */
	UPROPERTY()
	UMaterialInstance* WhiteMaterial;

	/** Pointer to current material used on active blocks */
	UPROPERTY()
	UMaterialInstance* CurrentMaterial = nullptr;

	/** Default color of a block is white */
	UPROPERTY()
	ETileColor CurrentColor = ETileColor::WHITE;

	/** Grid that owns us */
	UPROPERTY()
	class AMatch3LineDrawerBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	virtual void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	UFUNCTION()
	virtual void OverBlockEnter(UPrimitiveComponent* ClickedComp);

	UFUNCTION()
	virtual void BlockReleased(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	virtual void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	virtual void OnFingerEnterBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	virtual void OnFingerReleasedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void OnPointEvent();

	void OnOverEnterEvent();

	void OnReleasedEvent();

	void HandleSelection();

	bool IsLeftMouseButtonPressed();

	void SaveCurrentIndex();

	void RestoreLastIndex();

	bool IsReturning();

	void SelectBlock();

	void DeselectBlock();

	bool IsBlockSelectable() const;

	bool IsBlockSelected() const;

	void Highlight(bool bOn);

	bool IsAdjacent(int32 OtherIndex) const;

	bool IsSameColor(int32 OtherIndex) const;

	void SetIndex(int32 index);

	void UpdateIndexText();

	ETileColor GetCurrentColor();

	UFUNCTION()
	class AMatch3LineDrawerPlayerController* GetMatch3LineDrawerPlayerController() const;

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};

