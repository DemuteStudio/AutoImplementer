// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GisbImportContainerBase.h"
#include "GISB_MetasoundBuilder.generated.h"

/**
 * 
 */
UCLASS()
class GISBUNREAL_API UGISB_MetasoundBuilder : public UObject
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Metasound")
	static UMetaSoundSource* CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path);
};
