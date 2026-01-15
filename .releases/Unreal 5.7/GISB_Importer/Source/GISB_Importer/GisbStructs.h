// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "GisbStructs.generated.h"

// Trigger container transition mode
UENUM(BlueprintType)
enum class EGisbTriggerTransitionMode : uint8
{
	TriggerRate = 0   UMETA(DisplayName = "Trigger Rate"),
	Crossfade = 1     UMETA(DisplayName = "Crossfade")
};

USTRUCT(BlueprintType)
struct FGisbAttenuation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	bool overrideParent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	TObjectPtr<USoundAttenuation> soundAttenuation = nullptr;
};

USTRUCT(BlueprintType)
struct FGisbVolume
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float maxRnd = 0;
};

USTRUCT(BlueprintType)
struct FGisbPitch
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float maxRnd = 0;
};

USTRUCT(BlueprintType)
struct FGisbLowPass
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	bool randomize = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float minRnd = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
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
