// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3LineDrawerBlock.h"
#include "Match3LineDrawerBlockGrid.h"
#include "Match3LineDrawerPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"
#include "Math/UnrealMathUtility.h"
#include "DA_BlockMeshHex.h"
#include "GameFramework/PlayerInput.h"

#define LOCTEXT_NAMESPACE "PuzzleBlock"

AMatch3LineDrawerBlock::AMatch3LineDrawerBlock()
{
	//TODO: Expose materials to Blueprints and use Data Assets and Asset Manager instead of hardcoded materials.
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> PurpleMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> WhiteMaterial;

		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/SM_PuzzleHexHorizontal.SM_PuzzleHexHorizontal"))
			, BaseMaterial(TEXT("/Game/Puzzle/Materials/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Materials/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Materials/OrangeMaterial.OrangeMaterial"))
			, RedMaterial(TEXT("/Game/Puzzle/Materials/RedMaterial.RedMaterial"))
			, GreenMaterial(TEXT("/Game/Puzzle/Materials/GreenMaterial.GreenMaterial"))
			, PurpleMaterial(TEXT("/Game/Puzzle/Materials/PurpleMaterial.PurpleMaterial"))
			, WhiteMaterial(TEXT("/Game/Puzzle/Materials/WhiteMaterial.WhiteMaterial"))
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

	//Bind mouse input events handlers
	BlockMesh->OnClicked.AddDynamic(this, &AMatch3LineDrawerBlock::BlockClicked);
	BlockMesh->OnBeginCursorOver.AddDynamic(this, &AMatch3LineDrawerBlock::OverBlockEnter);
	BlockMesh->OnReleased.AddDynamic(this, &AMatch3LineDrawerBlock::BlockReleased);

	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMatch3LineDrawerBlock::OnFingerPressedBlock);
	BlockMesh->OnInputTouchEnter.AddDynamic(this, &AMatch3LineDrawerBlock::OnFingerEnterBlock);
	BlockMesh->OnInputTouchEnd.AddDynamic(this, &AMatch3LineDrawerBlock::OnFingerReleasedBlock);

	// Save pointers to the colored materials
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	GreenMaterial = ConstructorStatics.GreenMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	PurpleMaterial = ConstructorStatics.PurpleMaterial.Get();
	WhiteMaterial = ConstructorStatics.WhiteMaterial.Get();

	// Create static mesh component
	IndexText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IndexText0"));

	if (blockDataAsset)
	{
		FVector relativeTextLocation = blockDataAsset->GetIndexTextRelativeOffset();
		IndexText->SetRelativeLocation(relativeTextLocation);
		FVector relativeScale3D = blockDataAsset->GetRelativeScale3D();
		IndexText->SetRelativeScale3D(relativeScale3D);
	}

	IndexText->SetRelativeRotation(FRotator(90.f, 0.f, 180.f));
	IndexText->SetText(FText::Format(LOCTEXT("IndexFmt", "{0}"), FText::AsNumber(0)));
	IndexText->SetupAttachment(DummyRoot);
}

UMaterialInstance* AMatch3LineDrawerBlock::SelectMaterial(const ETileColor& Color)
{
	UMaterialInstance* materialInstance = nullptr;
	switch (Color)
	{
	case ETileColor::RED:
		materialInstance = RedMaterial;
		break;
	case ETileColor::GREEN:
		materialInstance = GreenMaterial;
		break;
	case ETileColor::BLUE:
		materialInstance = BlueMaterial;
		break;
	case ETileColor::ORANGE:
		materialInstance = OrangeMaterial;
		break;
	case ETileColor::PURPLE:
		materialInstance = PurpleMaterial;
		break;
	case ETileColor::WHITE:
		materialInstance = WhiteMaterial;
		break;
	default:
		materialInstance = WhiteMaterial;
		break;
	}

	BlockMesh->SetMaterial(0, materialInstance);
	CurrentMaterial = materialInstance;
	return materialInstance;
}

int32 AMatch3LineDrawerBlock::GetLastSelectedBlockIndex() const
{
	return LastSelectedBlockIndex;
}

void AMatch3LineDrawerBlock::UpdateMaterial()
{
	SelectMaterial(CurrentColor);
}

ETileColor AMatch3LineDrawerBlock::SelectRandomColor()
{
	int32 randomIndex = FMath::RandRange(0, 4);
	ETileColor color;
	color = ETileColor::RED;
	switch (randomIndex)
	{
	case 0:
		color = ETileColor::RED;
		break;
	case 1:
		color = ETileColor::GREEN;
		break;
	case 2:
		color = ETileColor::BLUE;
		break;
	case 3:
		color = ETileColor::ORANGE;
		break;
	case 4:
		color = ETileColor::PURPLE;
		break;
	default:
		color = ETileColor::WHITE;
		break;
	}

	CurrentColor = color;
	return color;
}

void AMatch3LineDrawerBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OnPointEvent();
}

void AMatch3LineDrawerBlock::OverBlockEnter(UPrimitiveComponent* ClickedComp)
{
	OnOverEnterEvent();
}

void AMatch3LineDrawerBlock::BlockReleased(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OnReleasedEvent();
}

void AMatch3LineDrawerBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnPointEvent();
}

void AMatch3LineDrawerBlock::OnFingerEnterBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnOverEnterEvent();
}

void AMatch3LineDrawerBlock::OnFingerReleasedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	OnReleasedEvent();
}

void AMatch3LineDrawerBlock::OnPointEvent()
{
	HandleSelection();
}

void AMatch3LineDrawerBlock::OnOverEnterEvent()
{
	if (OwningGrid == nullptr)
	{
		return;
	}

	if (IsLeftMouseButtonPressed() && OwningGrid->SelectionEnabled)
	{
		UE_LOG(LogTemp, Log, TEXT("OverBlockEnter && LeftMouseButtonPressed"));
		if (IsBlockSelectable())
		{
			SelectBlock();
			UE_LOG(LogTemp, Log, TEXT("Block %d Selected"), Index);
		}
		else if (IsReturning())
		{
			OwningGrid->DeselectBlockOnReturn();
		}
	}
}

void AMatch3LineDrawerBlock::HandleSelection()
{
	if (OwningGrid != nullptr)
	{
		OwningGrid->SelectionEnabled = true;
	}

	if (IsBlockSelectable())
	{
		SelectBlock();
	}
	else if (IsBlockSelected())
	{
		DeselectBlock();
	}
}

bool AMatch3LineDrawerBlock::IsLeftMouseButtonPressed()
{
	if (AMatch3LineDrawerPlayerController* playerController = GetMatch3LineDrawerPlayerController())
	{
		if (playerController->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			return true;
		}
	}

	return false;

}

void AMatch3LineDrawerBlock::SaveCurrentIndex()
{
	if (OwningGrid != nullptr)
	{
		LastSelectedBlockIndex = OwningGrid->GetLastSelectedBlockIndex();
		OwningGrid->SetLastSelectedBlockIndex(Index);
	}
}

void AMatch3LineDrawerBlock::RestoreLastIndex()
{
	if (OwningGrid != nullptr)
	{
		OwningGrid->SetLastSelectedBlockIndex(LastSelectedBlockIndex);
		LastSelectedBlockIndex = -1;
	}
}

bool AMatch3LineDrawerBlock::IsReturning()
{
	if (OwningGrid == nullptr)
	{
		return false;
	}

	if (AMatch3LineDrawerBlock* LastSelectedTile = OwningGrid->GetLastSelectedBlock())
	{
		if (LastSelectedTile->LastSelectedBlockIndex == Index)
		{
			return true;
		}
	}

	return false;
}

void AMatch3LineDrawerBlock::SelectBlock()
{
	if (IsBlockSelected())
	{
		return;
	}

	bIsActive = true;
	SaveCurrentIndex();

	// Change material to highlighted base material
	BlockMesh->SetMaterial(0, BaseMaterial);

	if (OwningGrid != nullptr)
	{
		OwningGrid->AddSelectedTile(Index);
	}
}

void AMatch3LineDrawerBlock::DeselectBlock()
{
	if (!IsBlockSelected())
	{
		return;
	}

	if (OwningGrid != nullptr)
	{
		OwningGrid->RemoveSelectedTile(Index);
	}

	BlockMesh->SetMaterial(0, CurrentMaterial);

	RestoreLastIndex();

	bIsActive = false;
}

void AMatch3LineDrawerBlockGrid::HideBlock()
{
	SetActorHiddenInGame(true);
}

bool AMatch3LineDrawerBlock::IsBlockSelectable() const
{
	if (bIsActive)
	{
		return false;
	}

	if (!OwningGrid)
	{
		return false;
	}

	const int32 PreviouslySelectedBlockIndex = OwningGrid->GetLastSelectedBlockIndex();
	if (PreviouslySelectedBlockIndex != -1)
	{
		if (!IsAdjacent(PreviouslySelectedBlockIndex))
		{
			return false;
		}

		if (!IsSameColor(PreviouslySelectedBlockIndex))
		{
			return false;
		}
	}

	return true;
}

bool AMatch3LineDrawerBlock::IsBlockSelected() const
{
	if (bIsActive)
	{
		return true;
	}

	return false;
}

void AMatch3LineDrawerBlock::OnReleasedEvent()
{
	if (OwningGrid != nullptr)
	{
		OwningGrid->EvaluateTilesSelection();
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

bool AMatch3LineDrawerBlock::IsAdjacent(int32 OtherIndex) const
{
	if (Index == -1)
	{
		return false;
	}

	if (OtherIndex == -1)
	{
		return false;
	}

	if (OwningGrid == nullptr)
	{
		return false;
	}

	const int32 sizeHorizontal = OwningGrid->SizeHorizontal;

	int32 column = Index % sizeHorizontal;
	int32 otherColumn = OtherIndex % sizeHorizontal;
	float row = Index / sizeHorizontal;
	float otherRow = OtherIndex / sizeHorizontal;

	// Filtering by maximum row and column distance of 1 between the tiles would give us 8 different neighbors as in a rectangular grid.
	// As we get hexagonal grid by vertically shifting the odd columns by half the tile size, we apply that to the distance filter,
	// as additional 2 diagonal "neighbors" get separated by the distance of 1.5, which gives us final neighbors size of 6.
	if (column % 2 == 1)
	{
		row += 0.5f;
	}

	if (otherColumn % 2 == 1)
	{
		otherRow += 0.5f;
	}

	if (FMath::Abs(column - otherColumn) > 1)
	{
		return false;
	}

	if (FMath::Abs(row - otherRow) > 1)
	{
		return false;
	}

	return true;

}

bool AMatch3LineDrawerBlock::IsSameColor(int32 OtherIndex) const
{
	if (!OwningGrid)
	{
		return false;
	}

	if (AMatch3LineDrawerBlock* otherTile = OwningGrid->GetTile(OtherIndex))
	{
		ETileColor otherColor = otherTile->GetCurrentColor();
		if (CurrentColor == otherColor)
		{
			return true;
		}
	}


	return false;
}

void AMatch3LineDrawerBlock::SetIndex(int32 IndexToSet)
{
	Index = IndexToSet;
	UpdateIndexText();
}

void AMatch3LineDrawerBlock::UpdateIndexText()
{
	IndexText->SetText(FText::Format(LOCTEXT("IndexFmt", "{0}"), FText::AsNumber(Index)));
}

ETileColor AMatch3LineDrawerBlock::GetCurrentColor()
{
	return CurrentColor;
}

AMatch3LineDrawerPlayerController* AMatch3LineDrawerBlock::GetMatch3LineDrawerPlayerController() const
{
	AMatch3LineDrawerPlayerController* playerController = nullptr;
	if (UWorld* world = GetWorld())
	{
		playerController = Cast<AMatch3LineDrawerPlayerController>(world->GetFirstPlayerController());
	}
	return playerController;
}