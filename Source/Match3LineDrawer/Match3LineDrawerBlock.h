// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Match3LineDrawerBlock.generated.h"

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
	
	/** Text component for displaying grid index */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* IndexText;

	int32 index = -1;

public:
	AMatch3LineDrawerBlock();

	class UMaterialInstance* SelectRandomMaterial();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	UMaterialInstance* OrangeMaterial;

	/** Pointer to red material used on active blocks */
	UPROPERTY()
	UMaterialInstance * RedMaterial;
	
	/** Pointer to green material used on active blocks */
	UPROPERTY()
	UMaterialInstance * GreenMaterial;
	
	/** Pointer to purple material used on active blocks */
	UPROPERTY()
	UMaterialInstance * PurpleMaterial;
	
	/** Pointer to current material used on active blocks */
	UPROPERTY()
	 UMaterialInstance * CurrentMaterial = nullptr;

	/** Grid that owns us */
	UPROPERTY()
	class AMatch3LineDrawerBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();

	void Highlight(bool bOn);

	bool IsAdjacent(int32 otherIndex, int32 gridSizeHorizontal = 7, int32 gridSizeVertical = 6);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};



