// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "UObject/NoExportTypes.h"
#include "GisbContainerRandom.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class GISBUNREAL_API UGisbContainerRandom : public UGisbContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<UGisbContainerBase*> ChildContainers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AvoidLastPlayed = 0;

	virtual void Play(UObject* WorldContext, const TMap<FName, FString>& Parameters) override;
};
