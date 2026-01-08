// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerRandom.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class UGisbImportContainerRandom : public UGisbImportContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GISB")
	TArray<TObjectPtr<UGisbImportContainerBase>> SoundArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	int AvoidLastPlayed = 0;

	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path) override;
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) override;
	virtual void ComputeCachedProperties() override;
};
