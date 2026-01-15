// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundFrontendDocument.h"
#include "TypeFamily/ChannelTypeFamily.h"

#include "MetasoundStringSwitchNode.generated.h"


USTRUCT()
struct FMetaSoundStringSwitchNodeConfiguration : public FMetaSoundFrontendNodeConfiguration
{
	GENERATED_BODY()

	FMetaSoundStringSwitchNodeConfiguration();

	UPROPERTY(EditAnywhere, Category= General)
	uint32 DefaultOut;

	UPROPERTY(EditAnywhere, Category = General)
	TArray<FString> SwitchValues;

	/* Get the current interface for the class based upon the node extension */
	virtual TInstancedStruct<FMetasoundFrontendClassInterface> OverrideDefaultInterface(const FMetasoundFrontendClass& InNodeClass) const;

	/* Pass data down to the operator. */
	virtual TSharedPtr<const Metasound::IOperatorData> GetOperatorData() const override;
};
