// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GisbImportContainerBase.h"
#include "GisbStructs.h"
#include "GisbImportContainerTrigger.generated.h"

/**
 * Import helper for trigger containers
 */
UCLASS(BlueprintType, EditInlineNew)
class GISB_IMPORTER_API UGisbImportContainerTrigger : public UGisbImportContainerBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	int32 TriggerAmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	EGisbTriggerTransitionMode TransitionMode = EGisbTriggerTransitionMode::TriggerRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	float TriggerRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GISB")
	float CrossfadeDuration = 1.0f;

	// Import container for the triggered child sound
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GISB")
	TObjectPtr<UGisbImportContainerBase> TriggeredSoundImport;

	// Parse JSON and populate this import container
	virtual void ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path) override;

	// Convert import container to runtime container
	virtual UGisbContainerBase* ToRuntimeContainer(UObject* Outer) override;
	virtual void ComputeCachedProperties() override;
};
