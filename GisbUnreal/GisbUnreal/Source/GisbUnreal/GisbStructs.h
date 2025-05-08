// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "GisbStructs.generated.h"

USTRUCT(BlueprintType)
struct FGisbAttenuation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool overrideParent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value")
	bool active = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minDistance = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxDistance = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)  
    FRuntimeFloatCurve attenuationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volumeAtMax = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spreadAtMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float spreadAtMax = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lowPassAtMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lowPassAtMax = 0;
};

USTRUCT(BlueprintType)
struct FGisbVolume
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Randomization")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRnd = 0;
};

USTRUCT(BlueprintType)
struct FGisbPitch
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Randomization")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRnd = 0;
};

USTRUCT(BlueprintType)
struct FGisbLowPass
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Randomization")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxRnd = 0;
};

/*USTRUCT(BlueprintType)
struct FGisbCurve
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGisbKey> keys;
	
};

USTRUCT(BlueprintType)
struct FGisbKey
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float time = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float inTangent = 0.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float outTangent = -1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float inWeight = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float outWeight = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int weightedMode = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int tangentMode = 0;
};*/
