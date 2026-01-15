// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbImportContainerBase.h"
#include "Engine/DataAsset.h"
#include "MetasoundSource.h"
#include "GisbSoundBankDataAsset.generated.h"

/**
 * 
 */
UCLASS()

class UGisbSoundBankDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	FString EventName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	TObjectPtr<UMetaSoundSource> GeneratedMS;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category="GISB")
	TObjectPtr<UGisbContainerBase> RootContainer;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset Info")
	FPrimaryAssetType AssetType;
};
