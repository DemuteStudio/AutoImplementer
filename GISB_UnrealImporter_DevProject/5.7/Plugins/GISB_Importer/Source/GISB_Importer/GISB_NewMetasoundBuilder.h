// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundEditorSubsystem.h"
#include "GISB_NewMetasoundBuilder.generated.h"

class UGisbSoundBankDataAsset;

UCLASS()
class GISB_IMPORTER_API UGISB_NewMetasoundBuilder : public UObject
{
	GENERATED_BODY()

public:
	// Test function for builder
	UFUNCTION(BlueprintCallable, Category = "ISX")
	static void TestBuilder(UGisbSoundBankDataAsset* dataAsset);

private:
	// Static node references (copied from original)
	static FMetasoundFrontendClassName* WavePlayerMonoNode;
	static FMetasoundFrontendClassName* WavePlayerStereoNode;
	static FMetasoundFrontendClassName* ProbabilityNode;
	static TArray<FMetasoundFrontendClassName*>* MonoMixerNodes;
	static TArray<FMetasoundFrontendClassName*>* StereoMixerNodes;
	static TArray<FMetasoundFrontendClassName*>* TriggerAnyNodes;
	static TArray<FMetasoundFrontendClassName*>* TriggerAccumulateNodes;
	static TScriptInterface<IMetaSoundDocumentInterface> AudioRerouteNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbRandomNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbSwitchNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbVolumeNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbPitchNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbLowpassNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbAttenuationNode;
	
	// Build simple sound node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildSimpleSoundNode(
		UGisbImportContainerSimpleSound* simpleSound,
		const FString& Name
	);

	// Setup probability variables for patch builder
	static void setupProbability(
		UMetaSoundPatchBuilder* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& playTrigger,
		FMetaSoundBuilderNodeInputHandle& onFinished
	);

	// Connect probability node to graph
	static void ConnectProbabilityNode(
		UMetaSoundPatchBuilder* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& executionHandle,
		FMetaSoundBuilderNodeInputHandle& finishHandle
	);

	// Connect volume effect to graph
	static void ConnectVolume(
		UMetaSoundPatchBuilder* builder,
		const FGisbVolume volume,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle
	);

	// Connect pitch effect to graph
	static void ConnectPitch(
		UMetaSoundPatchBuilder* builder,
		const FGisbPitch pitch,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle
	);

	// Connect lowpass effect to graph
	static void ConnectLowpass(
		UMetaSoundPatchBuilder* builder,
		const FGisbLowPass lowpass,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle
	);

	// Setup audio effect chain (Volume, Pitch, Lowpass)
	static void setupAttributes(
		UMetaSoundPatchBuilder* builder,
		UGisbImportContainerSimpleSound* simpleSound,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle
	);

	// Setup all node references
	static void SetupNodes();
};

// Global variables
extern FString PathGlobal;
extern UMetaSoundBuilderSubsystem* BuilderGlobal;
