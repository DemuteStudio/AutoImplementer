// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "GisbStructs.h"
#include "GisbContainerTrigger.generated.h"

/**
 * Trigger container - repeatedly triggers a child sound based on rate or crossfade timing
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced)
class GISB_IMPORTER_API UGisbContainerTrigger : public UGisbContainerBase
{
	GENERATED_BODY()

public:
	// Transition mode (TriggerRate or Crossfade)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	EGisbTriggerTransitionMode TransitionMode = EGisbTriggerTransitionMode::TriggerRate;

	// Time in seconds between triggers (used in TriggerRate mode)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float TriggerRate = 1.0f;

	// Number of times to trigger (-1 for infinite)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	int32 TriggerAmount = 1;

	// Crossfade duration in seconds (used in Crossfade mode)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GISB")
	float CrossfadeDuration = 1.0f;

	// The child sound that gets triggered
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GISB")
	TObjectPtr<UGisbContainerBase> TriggeredSound;

	// Override Play method (runtime logic - TODO)
	virtual void Play(UObject* WorldContext, const TMap<FName, FString>& Parameters) override;
};
