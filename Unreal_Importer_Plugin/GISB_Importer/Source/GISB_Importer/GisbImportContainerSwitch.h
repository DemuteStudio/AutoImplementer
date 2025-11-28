// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSwitch.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class UGisbImportContainerSwitch : public UGisbImportContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	FName ParameterID;
	FName DefaultParameterValue;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GISB")
	TMap<FString, UGisbImportContainerBase*> SoundDictionary;

	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path ) override;
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) override;
};
