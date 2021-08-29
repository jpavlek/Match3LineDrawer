// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerBlock.h"
#include "Match3LineDrawerBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Math/UnrealMathUtility.h"

AMatch3LineDrawerBlock::AMatch3LineDrawerBlock()
{
	//TODO: Use Data Assets and Asset Manager instead of hardcoded materials.
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> PurpleMaterial;

		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/SM_PuzzleHexHorizontal.SM_PuzzleHexHorizontal"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, RedMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
			, GreenMaterial(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
			, PurpleMaterial(TEXT("/Game/Puzzle/Meshes/PurpleMaterial.PurpleMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMatch3LineDrawerBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMatch3LineDrawerBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();
	PurpleMaterial = ConstructorStatics.PurpleMaterial.Get();
}

UMaterialInstance* AMatch3LineDrawerBlock::SelectRandomMaterial()
{
	int32 randomIndex = FMath::RandRange(0, 4);
	UMaterialInstance* materialInstance = nullptr;
	switch (randomIndex)
	{
	case 0:
		materialInstance = BlueMaterial;
		break;
	case 1:
		materialInstance = OrangeMaterial;
		break;
	case 2:
		materialInstance = RedMaterial;
		break;
	case 3:
		materialInstance = GreenMaterial;
		break;
	case 4:
		materialInstance = PurpleMaterial;
		break;
	default:
		materialInstance = BlueMaterial;
		break;
	}

	BlockMesh->SetMaterial(0, materialInstance);
	CurrentMaterial = materialInstance;
	return materialInstance;
}

void AMatch3LineDrawerBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMatch3LineDrawerBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMatch3LineDrawerBlock::HandleClicked()
{
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, BaseMaterial);

		// Tell the Grid
		if (OwningGrid != nullptr)
		{
			OwningGrid->AddScore();
		}
	}
	else
	{
		bIsActive = false;

		// Change material
		BlockMesh->SetMaterial(0, CurrentMaterial);
	}
}

void AMatch3LineDrawerBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, CurrentMaterial);
	}
}
