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
class UGisbContainerBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	FGisbAttenuation Attenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	FGisbVolume VolumeDB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	FGisbPitch Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	FGisbLowPass Lowpass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float PlaybackProbabilityPercent = 100;

	// Called to trigger playback
	virtual void Play(class UObject* WorldContext, const TMap<FName, FString>& Parameters) PURE_VIRTUAL(USoundContainerRuntime::Play, );
};
