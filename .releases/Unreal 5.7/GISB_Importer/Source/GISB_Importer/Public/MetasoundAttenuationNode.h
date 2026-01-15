// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundFrontendDocument.h"
#include "TypeFamily/ChannelTypeFamily.h"

#include "MetasoundAttenuationNode.generated.h"


USTRUCT()
struct FMetaSoundAttenuationNodeConfiguration : public FMetaSoundFrontendNodeConfiguration
{
	GENERATED_BODY()

	FMetaSoundAttenuationNodeConfiguration();

	UPROPERTY(EditAnywhere, Category= General)
	TObjectPtr<USoundAttenuation> SoundAttenuation;

	UPROPERTY(EditAnywhere, Category = General, meta = (ClampMin = "1", ClampMax = "1000"))
	uint32 NumInputs;

	/* Get the current interface for the class based upon the node extension */
	virtual TInstancedStruct<FMetasoundFrontendClassInterface> OverrideDefaultInterface(const FMetasoundFrontendClass& InNodeClass) const;

	/* Pass data down to the operator. */
	virtual TSharedPtr<const Metasound::IOperatorData> GetOperatorData() const override;
};
