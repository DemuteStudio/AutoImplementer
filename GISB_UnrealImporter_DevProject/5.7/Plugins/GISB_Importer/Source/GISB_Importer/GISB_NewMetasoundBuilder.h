// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerBlend.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundEditorSubsystem.h"
#include "GisbMetasoundLayoutManager.h"
#include "GISB_NewMetasoundBuilder.generated.h"

class UGisbSoundBankDataAsset;

// Result struct for recursive child patch building
struct FChildPatchResult
{
	TScriptInterface<IMetaSoundDocumentInterface> Patch;
	bool bIsStereo;

	FChildPatchResult() : Patch(nullptr), bIsStereo(false) {}
};

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

	// Build random container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildRandomNode(
		UGisbImportContainerRandom* randomContainer,
		const FString& Name
	);

	// Build switch container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildSwitchNode(
		UGisbImportContainerSwitch* switchContainer,
		const FString& Name
	);

	// Build blend container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildBlendNode(
		UGisbImportContainerBlend* blendContainer,
		const FString& Name
	);

	// Recursive child builder with stereo detection
	static FChildPatchResult BuildChildNode(
		UGisbImportContainerBase* container,
		const FString& ParentName,
		int32 ChildIndex
	);

	// Setup probability variables for patch builder
	static void setupProbability(
		UMetaSoundPatchBuilder* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& playTrigger,
		FMetaSoundBuilderNodeInputHandle& onFinished,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Connect probability node to graph
	static void ConnectProbabilityNode(
		UMetaSoundPatchBuilder* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& executionHandle,
		FMetaSoundBuilderNodeInputHandle& finishHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Connect volume effect to graph
	static void ConnectVolume(
		UMetaSoundPatchBuilder* builder,
		const FGisbVolume volume,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Connect pitch effect to graph
	static void ConnectPitch(
		UMetaSoundPatchBuilder* builder,
		const FGisbPitch pitch,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Connect lowpass effect to graph
	static void ConnectLowpass(
		UMetaSoundPatchBuilder* builder,
		const FGisbLowPass lowpass,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Setup audio effect chain (Volume, Pitch, Lowpass)
	static void setupAttributes(
		UMetaSoundPatchBuilder* builder,
		UGisbImportContainerSimpleSound* simpleSound,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// Setup all node references
	static void SetupNodes();
};

// Global variables
extern FString PathGlobal;
extern UMetaSoundBuilderSubsystem* BuilderGlobal;
