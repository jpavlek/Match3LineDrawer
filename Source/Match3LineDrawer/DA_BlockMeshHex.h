// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_BlockMeshHex.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MATCH3LINEDRAWER_API UDA_BlockMeshHex : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMeshHex = nullptr;

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector IndexTextRelativeOffset = FVector(0.f, 0.f, 50.f);

	UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector RelativeScale3D = FVector(2.f, 2.f, 2.f);

public:
	FVector GetIndexTextRelativeOffset();
	FVector GetRelativeScale3D();
};
