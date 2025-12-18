// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundBuilderBase.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerBlend.h"
#include "GisbStructs.h"
#include "MetasoundFrontendDocument.h"
#include "GISB_MetasoundBuilderCore.generated.h"

// Forward declarations
class GisbMetasoundLayoutManager;
struct FChildPatchResult;

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
	 * Recursively check if container or any of its children contain stereo audio.
	 */
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
	 * @param Layout Optional layout manager for node positioning
	 */
	static void BuildSimpleSoundCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerSimpleSound* simpleSound,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
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
	static void BuildRandomCore(
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
	static void BuildSwitchCore(
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
	 */
	static void BuildBlendCore(
		UMetaSoundBuilderBase* builder,
		UGisbImportContainerBlend* blendContainer,
		const FString& Name,
		FMetaSoundBuilderNodeOutputHandle& triggerInput,
		FMetaSoundBuilderNodeInputHandle& onFinishedInput,
		FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
		FMetaSoundBuilderNodeInputHandle* audioRightOutput,
		GisbMetasoundLayoutManager* Layout = nullptr
	);
};
