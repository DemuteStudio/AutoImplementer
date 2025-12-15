// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class UGisbImportContainerSimpleSound : public UGisbImportContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	FString AudioFilePath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	bool loop;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	TObjectPtr<USoundWave> SoundWave = nullptr;

	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path) override;
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) override;
};
