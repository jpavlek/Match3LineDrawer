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

		//UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//FName Color;

		UPROPERTY(Category = Block, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* BlockMeshHex = nullptr;

};