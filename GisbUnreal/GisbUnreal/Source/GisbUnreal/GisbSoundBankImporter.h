// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GisbSoundBankImporter.generated.h"

/**
 * 
 */
UCLASS()
class GISBUNREAL_API UGisbSoundBankImporter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "SoundBank")
	static void ImportSoundBankFromJson();
};
