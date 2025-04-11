// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "UObject/NoExportTypes.h"
#include "GisbContainerSwitch.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class GISBUNREAL_API UGisbContainerSwitch : public UGisbContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ParameterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ParameterValue;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TMap<FString, UGisbContainerBase*> SwitchMap;

	virtual void Play(UObject* WorldContext, const TMap<FName, FString>& Parameters) override;
};