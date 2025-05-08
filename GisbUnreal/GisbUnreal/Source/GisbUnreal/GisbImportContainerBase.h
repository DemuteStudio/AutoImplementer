// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GisbContainerBase.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GisbImportContainerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract, EditInlineNew)
class GISBUNREAL_API UGisbImportContainerBase : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	FString Type;

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

	static UGisbImportContainerBase* CreateFromJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path);

	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path) /*PURE_VIRTUAL(USoundContainerBase::ParseJson, )*/;
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) PURE_VIRTUAL(UGisbImportContainerBase::ToRuntimeContainer, return nullptr;);
	void AssignBaseVariables(UGisbContainerBase* Container) const;
};
