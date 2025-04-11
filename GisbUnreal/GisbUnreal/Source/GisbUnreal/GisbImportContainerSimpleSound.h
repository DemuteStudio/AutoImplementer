// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class GISBUNREAL_API UGisbImportContainerSimpleSound : public UGisbImportContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString AudioFilePath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool loop;
	
	UPROPERTY(EditAnywhere)
	USoundWave* SoundWave = nullptr;

	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path) override;
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) override;
};
