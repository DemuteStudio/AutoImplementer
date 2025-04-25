// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GisbStructs.h"
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
	FGisbAttenuation Attenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGisbVolume VolumeDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGisbPitch Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGisbLowPass Lowpass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlaybackProbabilityPercent;

	// Called to trigger playback
	virtual void Play(class UObject* WorldContext, const TMap<FName, FString>& Parameters) PURE_VIRTUAL(USoundContainerRuntime::Play, );
};
