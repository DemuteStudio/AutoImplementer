// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbImportContainerBase.h"
#include "Engine/DataAsset.h"
#include "GisbSoundBankDataAsset.generated.h"

/**
 * 
 */
UCLASS()

class GISBUNREAL_API UGisbSoundBankDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EventName;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category="Gisb")
	UGisbContainerBase* RootContainer;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset Info")
	FPrimaryAssetType AssetType;
};
