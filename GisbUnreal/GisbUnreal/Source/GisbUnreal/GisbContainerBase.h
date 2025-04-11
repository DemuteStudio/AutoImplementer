// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GisbContainerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class GISBUNREAL_API UGisbContainerBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EventName;

	// Called to trigger playback
	virtual void Play(class UObject* WorldContext, const TMap<FName, FString>& Parameters) PURE_VIRTUAL(USoundContainerRuntime::Play, );
};
