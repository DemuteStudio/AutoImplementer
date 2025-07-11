// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "UObject/NoExportTypes.h"
#include "GisbContainerBlend.generated.h"

/**
 *
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, Category = "GISB")
class UGisbContainerBlend : public UGisbContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GISB")
	TArray<TObjectPtr<UGisbContainerBase>> ChildContainers;

	virtual void Play(UObject* WorldContext, const TMap<FName, FString>& Parameters) override;
	
};
