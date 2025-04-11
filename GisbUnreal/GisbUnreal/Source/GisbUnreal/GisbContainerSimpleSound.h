// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "UObject/NoExportTypes.h"
#include "GisbContainerSimpleSound.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class GISBUNREAL_API UGisbContainerSimpleSound : public UGisbContainerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* Sound;
	bool loop;
	virtual void Play(UObject* WorldContext, const TMap<FName, FString>& Parameters) override;
};
