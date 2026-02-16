// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundBuilderBase.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerTrigger.h"
#include "GisbStructs.h"
#include "MetasoundFrontendDocument.h"
#include "GISB_MetasoundBuilderCore.generated.h"

// Forward declarations
class GisbMetasoundLayoutManager;
struct FChildPatchResult;
struct FGisbPinInfo;

/**
 * Base class for MetaSound builders (Patch and Source).
 * Contains shared helper methods and core build logic that work with UMetaSoundBuilderBase*.
 */
UCLASS()
class GISB_IMPORTER_API UGISB_MetasoundBuilderCore : public UObject
{
	GENERATED_BODY()

protected:
	// ============================================================================
	// Helper Methods (Builder-Agnostic)
	// ============================================================================

	/**
	 * Setup probability filtering for playback.
	 * If probability < 100%, adds a probability node that may skip execution.
	 */
	static void setupProbability(
		UMetaSoundBuilderBase* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& playTrigger,
		FMetaSoundBuilderNodeInputHandle& onFinished,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Setup audio effects chain (Volume, Pitch, Lowpass) based on container settings.
	 * Updates audio handles to point to processed outputs.
	 */
	static void setupAttributes(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerBase* container,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Connect probability node to graph.
	 * Splits execution into success/fail paths and merges them back with TriggerAny.
	 */
	static void ConnectProbabilityNode(
		UMetaSoundBuilderBase* builder,
		float probability,
		FMetaSoundBuilderNodeOutputHandle& executionHandle,
		FMetaSoundBuilderNodeInputHandle& finishHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Connect volume effect node to graph.
	 * Applies volume attenuation with optional randomization.
	 * Converts dB values to linear scale.
	 */
	static void ConnectVolume(
		UMetaSoundBuilderBase* builder,
		const FGisbVolume volume,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Connect pitch effect node to graph.
	 * Applies pitch shifting with optional randomization.
	 * Converts cents to semitones.
	 */
	static void ConnectPitch(
		UMetaSoundBuilderBase* builder,
		const FGisbPitch pitch,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Connect lowpass filter effect node to graph.
	 * Applies frequency filtering with optional randomization.
	 * Converts normalized values to Hz.
	 */
	static void ConnectLowpass(
		UMetaSoundBuilderBase* builder,
		const FGisbLowPass lowpass,
		const bool isStereo,
		FMetaSoundBuilderNodeOutputHandle* executionHandle,
		FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
		FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Connect time remaining calculation node to graph.
	 * Takes playback time (Time type) from wave player and calculates remaining time.
	 *
	 * @param builder The MetaSound builder
	 * @param soundWave The sound wave asset reference
	 * @param playbackTimeOutput Wave player's Playback Time output (Time type)
	 * @param timeRemainingOutput Optional graph output for Time Remaining (Time type, nullable)
	 * @param Layout Optional layout manager for node positioning
	 */
	static void ConnectTimeRemaining(
		UMetaSoundBuilderBase* builder,
		USoundWave* soundWave,
		FMetaSoundBuilderNodeOutputHandle& playbackTimeOutput,
		FMetaSoundBuilderNodeInputHandle* timeRemainingOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Recursively check if container or any of its children contain stereo audio.
	 * @deprecated Use container->bIsStereo cached property instead.
	 */
	UE_DEPRECATED(5.7, "Use container->bIsStereo cached property instead")
	static bool isStereo(UGisbImportContainerBase* container);

	// ============================================================================
	// Core Build Logic Methods
	// ============================================================================

	/**
	 * Core logic for building simple sound nodes.
	 * Handles wave player setup, probability, audio effects, and automatic output connection.
	 * Does not create graph inputs/outputs (caller's responsibility).
	 *
	 * @param builder The MetaSound builder (patch or source)
	 * @param simpleSound Simple sound container data
	 * @param triggerInput Input trigger handle (from graph input or previous node)
	 * @param onFinishedInput Output trigger handle (to graph output or next node)
	 * @param audioLeftOutput Graph audio left/mono output node (connected automatically)
	 * @param audioRightOutput Graph audio right output node (connected automatically if stereo, nullable)
	 * @param timeRemainingOutput Optional time remaining output (connected if provided, nullable)
	 * @param Layout Optional layout manager for node positioning
	 */
	static void BuildSimpleSoundCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerSimpleSound* simpleSound,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		FMetaSoundBuilderNodeInputHandle* timeRemainingOutput = nullptr,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Core logic for building random container nodes.
	 * Handles child patch building, random selector, mixer, TriggerAny, effects, and automatic output connection.
	 * Does not create graph inputs/outputs (caller's responsibility).
	 * Extracts parameters from container (avoidLastPlayed, playbackProbability, isStereo).
	 *
	 * @param builder The MetaSound builder (patch or source)
	 * @param randomContainer Random container with children and settings
	 * @param Name Base name for child asset generation
	 * @param triggerInput Input trigger handle
	 * @param onFinishedInput Output trigger handle
	 * @param audioLeftOutput Graph audio left/mono output node (connected automatically)
	 * @param audioRightOutput Graph audio right output node (connected automatically if stereo, nullable)
	 * @param Layout Optional layout manager
	 */
	/**
	 * @return Array of propagated child inputs
	 */
	static TArray<FGisbPinInfo> BuildRandomCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerRandom* randomContainer,
		const FString& Name,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Core logic for building switch container nodes.
	 * Builds children internally, applies probability filtering, switch selector, mixer, TriggerAny,
	 * audio effects, and automatic output connection.
	 * Does not create graph inputs/outputs (caller's responsibility).
	 *
	 * @param builder The MetaSound builder (patch or source)
	 * @param switchContainer Switch container with children and settings
	 * @param Name Base name for child asset generation
	 * @param parameterInput Switch parameter input handle (string type, from graph input)
	 * @param triggerInput Input trigger handle
	 * @param onFinishedInput Output trigger handle
	 * @param audioLeftOutput Graph audio left/mono output node (connected automatically)
	 * @param audioRightOutput Graph audio right output node (connected automatically if stereo, nullable)
	 * @param Layout Optional layout manager
	 */
	/**
	 * @return Array of propagated child inputs (does NOT include Switch Parameter - that's created by Build SwitchNode)
	 */
	static TArray<FGisbPinInfo> BuildSwitchCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerSwitch* switchContainer,
		const FString& Name,
		FMetaSoundBuilderNodeOutputHandle& parameterInput,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Core logic for building blend container nodes.
	 * Builds children internally, applies probability filtering, child mixing, TriggerAccumulate,
	 * audio effects, and automatic output connection.
	 * Does not create graph inputs/outputs (caller's responsibility).
	 *
	 * @param builder The MetaSound builder (patch or source)
	 * @param blendContainer Blend container with children and settings
	 * @param Name Base name for child asset generation
	 * @param triggerInput Input trigger handle
	 * @param onFinishedInput Output trigger handle
	 * @param audioLeftOutput Graph audio left/mono output node (connected automatically)
	 * @param audioRightOutput Graph audio right output node (connected automatically if stereo, nullable)
	 * @param Layout Optional layout manager
	 * @return Array of propagated child inputs
	 */
	static TArray<FGisbPinInfo> BuildBlendCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerBlend* blendContainer,
		const FString& Name,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	/**
	 * Core logic for building trigger container nodes.
	 * Uses TriggerRepeat node to repeatedly trigger child sound at fixed rate.
	 * Uses TriggerStopper node to limit On Finished signal based on TriggerAmount (when != -1).
	 * TODO: Add crossfade mode support.
	 *
	 * @param builder The MetaSound builder (patch or source)
	 * @param triggerContainer Trigger container with child and settings
	 * @param Name Base name for child asset generation
	 * @param triggerInput Input trigger handle
	 * @param onFinishedInput Output trigger handle
	 * @param audioLeftOutput Graph audio left/mono output node (connected automatically)
	 * @param audioRightOutput Graph audio right output node (connected automatically if stereo, nullable)
	 * @param Layout Optional layout manager
	 * @return Array of all inputs created (Break + any propagated from children)
	 */
	static TArray<FGisbPinInfo> BuildTriggerCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerTrigger* triggerContainer,
		const FString& Name,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);

	// ============================================================================
	// Parameter Propagation Helpers
	// ============================================================================

	/**
	 * Detect unconnected INPUT pins on an instantiated patch node.
	 * Call this AFTER making standard connections (Play, Audio, On Finished).
	 * Returns only unconnected inputs (skips outputs).
	 *
	 * @param nodeHandle The instantiated child patch node
	 * @param builder The MetaSound builder
	 * @param standardInputsToFilter Names of inputs to skip (e.g., "Play")
	 * @return Array of unconnected input pin metadata
	 */
	static TArray<FGisbPinInfo> DetectUnconnectedInputs(
		const FMetaSoundNodeHandle& nodeHandle,
		UMetaSoundBuilderBase* builder,
		const TSet<FName>& standardInputsToFilter
	);

	/**
	 * Merge unconnected inputs from multiple children.
	 * If multiple children have same parameter name+type, returns single entry.
	 * If same name but different types, logs error and skips.
	 *
	 * @param childNodes Array of instantiated child node handles
	 * @param builder The MetaSound builder
	 * @param standardInputsToFilter Names of inputs to skip
	 * @return Deduplicated array of inputs to propagate to parent
	 */
	static TArray<FGisbPinInfo> MergeChildInputs(
		const TArray<FMetaSoundNodeHandle>& childNodes,
		UMetaSoundBuilderBase* builder,
		const TSet<FName>& standardInputsToFilter
	);

	/**
	 * Create parent graph inputs for propagated pins and connect to children.
	 * For each pin, creates ONE parent input and connects to all children that have it.
	 *
	 * @param inputsToPropagate Deduplicated inputs from MergeChildInputs
	 * @param childNodes Array of instantiated child patch nodes
	 * @param builder The MetaSound builder (patch or source)
	 * @param Layout Optional layout manager
	 * @return Map of pin name -> parent graph input handle
	 */
	static TMap<FName, FMetaSoundBuilderNodeOutputHandle> CreateAndConnectPropagatedInputs(
		const TArray<FGisbPinInfo>& inputsToPropagate,
		const TArray<FMetaSoundNodeHandle>& childNodes,
		UMetaSoundBuilderBase* builder,
		GisbMetasoundLayoutManager* Layout
	);
};
