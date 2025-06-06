// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GisbEvent.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced, CollapseCategories, DisplayName = "Single Item")
class UGisbEvent : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	TSoftClassPtr<class UGisbContainerBase> ContainerClass;
};
