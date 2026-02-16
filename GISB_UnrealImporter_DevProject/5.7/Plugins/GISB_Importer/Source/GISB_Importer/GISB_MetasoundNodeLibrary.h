// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundFrontend.h"
#include "GISB_MetasoundNodeLibrary.generated.h"

/**
 * Static library for shared MetaSound node references.
 * Provides centralized access to commonly used MetaSound node class names and patches.
 * Eliminates code duplication between GISB_MetasoundPatchBuilder and GISB_MetasoundSourceBuilder.
 */
UCLASS()
class GISB_IMPORTER_API UGISB_MetasoundNodeLibrary : public UObject
{
	GENERATED_BODY()

public:
	// Initialize all node references (must be called before using any nodes)
	static void SetupNodes();

	// Wave Player Nodes
	static FMetasoundFrontendClassName* WavePlayerMonoNode;
	static FMetasoundFrontendClassName* WavePlayerStereoNode;

	// Trigger/Control Flow Nodes
	static FMetasoundFrontendClassName* ProbabilityNode;
	static FMetasoundFrontendClassName* TriggerRepeatNode;
	static FMetasoundFrontendClassName* TriggerCounterNode;
	static TArray<FMetasoundFrontendClassName*>* TriggerAnyNodes;
	static TArray<FMetasoundFrontendClassName*>* TriggerAccumulateNodes;

	// Mixer Nodes
	static TArray<FMetasoundFrontendClassName*>* MonoMixerNodes;
	static TArray<FMetasoundFrontendClassName*>* StereoMixerNodes;

	// Custom GISB Patch Nodes
	static TScriptInterface<IMetaSoundDocumentInterface> GisbRandomNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbVolumeNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbPitchNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbLowpassNode;
	
	//Custom GISB Crossfade Nodes
	static TScriptInterface<IMetaSoundDocumentInterface> GisbTimedCrossfadeMono;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbTimedCrossfadeStereo;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbCrossfadeTimer;
	
	//Custom GISB Nodes
	static TScriptInterface<IMetaSoundDocumentInterface> GisbTriggerStopper;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbTimeRemaining;
	
	//Custom Configurable Nodes
	static FMetasoundFrontendClassName* GisbSwitchNode;
	static FMetasoundFrontendClassName* GisbAttenuationNode;
};
