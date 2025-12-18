// Copyright Demute SRL. All Rights Reserved.

#include "GISB_MetasoundBuilderCore.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundFrontendDocument.h"
#include "GisbMetasoundLayoutManager.h"
#include "GISB_MetasoundNodeLibrary.h"
#include "GISB_MetasoundPatchBuilder.h"
#include <cmath>

// ============================================================================
// Helper Methods (Moved from GISB_MetasoundPatchBuilder)
// ============================================================================

void UGISB_MetasoundBuilderCore::setupProbability(
	UMetaSoundBuilderBase* builder,
	float probability,
	FMetaSoundBuilderNodeOutputHandle& playTrigger,
	FMetaSoundBuilderNodeInputHandle& onFinished,
	GisbMetasoundLayoutManager* Layout
)
{
	if (probability > 99.99f)
	{
		return;
	}

	ConnectProbabilityNode(builder, probability, playTrigger, onFinished, Layout);
}

void UGISB_MetasoundBuilderCore::setupAttributes(
	UMetaSoundBuilderBase* builder,
	UGisbImportContainerBase* container,
	const bool bisStereo,
	FMetaSoundBuilderNodeOutputHandle* executionHandle,
	FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
	FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
	GisbMetasoundLayoutManager* Layout
)
{
	// Apply Volume effect if configured
	if (container->VolumeDB.randomize || container->VolumeDB.value != 0)
	{
		ConnectVolume(builder, container->VolumeDB, bisStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}

	// Apply Pitch effect if configured
	if (container->Pitch.randomize || container->Pitch.value != 0)
	{
		ConnectPitch(builder, container->Pitch, bisStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}

	// Apply Lowpass effect if configured
	if (container->Lowpass.randomize || container->Lowpass.value != 0)
	{
		ConnectLowpass(builder, container->Lowpass, bisStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}
}

void UGISB_MetasoundBuilderCore::ConnectProbabilityNode(
	UMetaSoundBuilderBase* builder,
	float probability,
	FMetaSoundBuilderNodeOutputHandle& executionHandle,
	FMetaSoundBuilderNodeInputHandle& finishHandle,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	// Add Trigger Filter (Probability) node
	FMetaSoundNodeHandle probabilityHandle;
	probabilityHandle = builder->AddNodeByClassName(*UGISB_MetasoundNodeLibrary::ProbabilityNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !probabilityHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to add Trigger Filter node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(probabilityHandle, EGisbNodeCategory::TriggerFlow, TEXT("ProbabilityFilter"));
	}

	// Find input pins on probability node
	FMetaSoundBuilderNodeInputHandle floatHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Probability"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Probability input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle triggerHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Trigger"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Trigger input"));
		return;
	}

	// Set probability value (convert percentage to 0-1 range)
	FAudioParameter floatParam = FAudioParameter(TEXT("Probability"), probability * 0.01f);
	FMetasoundFrontendLiteral floatValue = FMetasoundFrontendLiteral(floatParam);
	builder->SetNodeInputDefault(floatHandle, floatValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to set Probability value"));
		return;
	}

	// Connect execution trigger to probability node
	builder->ConnectNodes(executionHandle, triggerHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to connect execution trigger"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(executionHandle.NodeID), probabilityHandle);
	}

	// Find output pins on probability node
	FMetaSoundBuilderNodeOutputHandle headsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Heads"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Heads output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle tailsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Tails"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Tails output"));
		return;
	}

	// Update executionHandle to point to Heads output (success path)
	executionHandle = headsHandle;

	// Add Trigger Any node to merge success/fail paths back to finish
	FMetaSoundNodeHandle anyHandle;
	anyHandle = builder->AddNodeByClassName(*((*UGISB_MetasoundNodeLibrary::TriggerAnyNodes)[0]), result);
	if (result != EMetaSoundBuilderResult::Succeeded || !anyHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to add Trigger Any node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(anyHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAny"));
	}

	// Connect Tails (fail) to Trigger Any Input 1
	FMetaSoundBuilderNodeInputHandle failHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 1"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Trigger Any In 1 input"));
		return;
	}

	builder->ConnectNodes(tailsHandle, failHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to connect Tails to Trigger Any"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(tailsHandle.NodeID), anyHandle);
	}

	// Connect Trigger Any output to original finish handle
	FMetaSoundBuilderNodeOutputHandle outHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Trigger Any Out output"));
		return;
	}

	builder->ConnectNodes(outHandle, finishHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to connect Trigger Any to finish"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(anyHandle, FMetaSoundNodeHandle(finishHandle.NodeID));
	}

	// Update finishHandle to point to Trigger Any Input 0 (for success path)
	FMetaSoundBuilderNodeInputHandle succesHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 0"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Trigger Any In 0 input"));
		return;
	}

	finishHandle = succesHandle;
}

void UGISB_MetasoundBuilderCore::ConnectVolume(
	UMetaSoundBuilderBase* builder,
	const FGisbVolume volume,
	const bool bisStereo,
	FMetaSoundBuilderNodeOutputHandle* executionHandle,
	FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
	FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	// Add Volume node
	FMetaSoundNodeHandle volumeHandle = builder->AddNode(UGISB_MetasoundNodeLibrary::GisbVolumeNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !volumeHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to add Volume node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(volumeHandle, EGisbNodeCategory::ParameterProcessor, TEXT("Volume"));
	}

	// Find execution input handle
	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Exec"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find Exec input"));
		return;
	}

	// Find audio input handles
	FMetaSoundBuilderNodeInputHandle inLeftHandle = builder->FindNodeInputByName(volumeHandle, TEXT("InL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find InL input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle inRightHandle = builder->FindNodeInputByName(volumeHandle, TEXT("InR"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find InR input"));
		return;
	}

	// Connect execution flow if provided
	if (executionHandle != nullptr)
	{
		builder->ConnectNodes(*executionHandle, execHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to connect execution"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(executionHandle->NodeID), volumeHandle);
		}
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to connect left audio input"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(firstAudioHandle.NodeID), volumeHandle);
	}

	if (bisStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to connect right audio input"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(secondAudioHandle.NodeID), volumeHandle);
		}
	}

	// Set parameters with dB to linear conversion: linearValue = 10^(dB * 0.05)
	float linearMax = FMath::Pow(10.0f, volume.maxRnd * 0.05f);
	FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(volumeHandle, FName("Max"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find Max input"));
		return;
	}
	FAudioParameter maxParam = FAudioParameter(TEXT("Max"), linearMax);
	FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
	builder->SetNodeInputDefault(maxHandle, maxValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to set Max parameter"));
		return;
	}

	float linearMin = FMath::Pow(10.0f, volume.minRnd * 0.05f);
	FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(volumeHandle, FName("Min"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find Min input"));
		return;
	}
	FAudioParameter minParam = FAudioParameter(TEXT("Min"), linearMin);
	FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
	builder->SetNodeInputDefault(minHandle, minValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to set Min parameter"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(volumeHandle, FName("Randomize"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find Randomize input"));
		return;
	}
	FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), volume.randomize);
	FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
	builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to set Randomize parameter"));
		return;
	}

	float linearValue = FMath::Pow(10.0f, volume.value * 0.05f);
	FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(volumeHandle, FName("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find Value input"));
		return;
	}
	FAudioParameter valueParam = FAudioParameter(TEXT("Value"), linearValue);
	FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
	builder->SetNodeInputDefault(valueHandle, valueValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to set Value parameter"));
		return;
	}

	// Find and assign output handles directly
	firstAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find OutL output"));
		return;
	}

	if (bisStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find OutR output"));
			return;
		}
	}
}

void UGISB_MetasoundBuilderCore::ConnectPitch(
	UMetaSoundBuilderBase* builder,
	const FGisbPitch pitch,
	const bool bisStereo,
	FMetaSoundBuilderNodeOutputHandle* executionHandle,
	FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
	FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	// Add Pitch node
	FMetaSoundNodeHandle pitchHandle = builder->AddNode(UGISB_MetasoundNodeLibrary::GisbPitchNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !pitchHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to add Pitch node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(pitchHandle, EGisbNodeCategory::ParameterProcessor, TEXT("Pitch"));
	}

	// Find execution input handle
	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Exec"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find Exec input"));
		return;
	}

	// Find audio input handles
	FMetaSoundBuilderNodeInputHandle inLeftHandle = builder->FindNodeInputByName(pitchHandle, TEXT("InL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find InL input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle inRightHandle = builder->FindNodeInputByName(pitchHandle, TEXT("InR"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find InR input"));
		return;
	}

	// Connect execution flow if provided
	if (executionHandle != nullptr)
	{
		builder->ConnectNodes(*executionHandle, execHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to connect execution"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(executionHandle->NodeID), pitchHandle);
		}
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to connect left audio input"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(firstAudioHandle.NodeID), pitchHandle);
	}

	if (bisStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to connect right audio input"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(secondAudioHandle.NodeID), pitchHandle);
		}
	}

	// Set parameters with cents to semitones conversion: semitones = cents * 0.01
	float semitonesMax = pitch.maxRnd * 0.01f;
	FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(pitchHandle, FName("Max"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find Max input"));
		return;
	}
	FAudioParameter maxParam = FAudioParameter(TEXT("Max"), semitonesMax);
	FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
	builder->SetNodeInputDefault(maxHandle, maxValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to set Max parameter"));
		return;
	}

	float semitonesMin = pitch.minRnd * 0.01f;
	FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(pitchHandle, FName("Min"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find Min input"));
		return;
	}
	FAudioParameter minParam = FAudioParameter(TEXT("Min"), semitonesMin);
	FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
	builder->SetNodeInputDefault(minHandle, minValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to set Min parameter"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(pitchHandle, FName("Randomize"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find Randomize input"));
		return;
	}
	FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), pitch.randomize);
	FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
	builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to set Randomize parameter"));
		return;
	}

	float semitonesValue = pitch.value * 0.01f;
	FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(pitchHandle, FName("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find Value input"));
		return;
	}
	FAudioParameter valueParam = FAudioParameter(TEXT("Value"), semitonesValue);
	FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
	builder->SetNodeInputDefault(valueHandle, valueValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to set Value parameter"));
		return;
	}

	// Find and assign output handles directly
	firstAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find OutL output"));
		return;
	}

	if (bisStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find OutR output"));
			return;
		}
	}
}

void UGISB_MetasoundBuilderCore::ConnectLowpass(
	UMetaSoundBuilderBase* builder,
	const FGisbLowPass lowpass,
	const bool bisStereo,
	FMetaSoundBuilderNodeOutputHandle* executionHandle,
	FMetaSoundBuilderNodeOutputHandle& firstAudioHandle,
	FMetaSoundBuilderNodeOutputHandle& secondAudioHandle,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	// Add Lowpass node
	FMetaSoundNodeHandle lowpassHandle = builder->AddNode(UGISB_MetasoundNodeLibrary::GisbLowpassNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !lowpassHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to add Lowpass node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(lowpassHandle, EGisbNodeCategory::ParameterProcessor, TEXT("Lowpass"));
	}

	// Find execution input handle
	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Exec"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find Exec input"));
		return;
	}

	// Find audio input handles
	FMetaSoundBuilderNodeInputHandle inLeftHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("InL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find InL input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle inRightHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("InR"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find InR input"));
		return;
	}

	// Connect execution flow if provided
	if (executionHandle != nullptr)
	{
		builder->ConnectNodes(*executionHandle, execHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to connect execution"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(executionHandle->NodeID), lowpassHandle);
		}
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to connect left audio input"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(firstAudioHandle.NodeID), lowpassHandle);
	}

	if (bisStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to connect right audio input"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(secondAudioHandle.NodeID), lowpassHandle);
		}
	}

	// Set parameters with inverted frequency conversion: frequency = (100 - normalized) * 219.80 + 20
	float frequencyMax = lowpass.maxRnd * 219.80f;
	FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(lowpassHandle, FName("Max"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find Max input"));
		return;
	}
	FAudioParameter maxParam = FAudioParameter(TEXT("Max"), frequencyMax);
	FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
	builder->SetNodeInputDefault(maxHandle, maxValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to set Max parameter"));
		return;
	}

	float frequencyMin = lowpass.minRnd * 219.80f;
	FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(lowpassHandle, FName("Min"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find Min input"));
		return;
	}
	FAudioParameter minParam = FAudioParameter(TEXT("Min"), frequencyMin);
	FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
	builder->SetNodeInputDefault(minHandle, minValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to set Min parameter"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(lowpassHandle, FName("Randomize"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find Randomize input"));
		return;
	}
	FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), lowpass.randomize);
	FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
	builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to set Randomize parameter"));
		return;
	}

	float clampedValue = FMath::Clamp(lowpass.value, 0.0f, 100.0f);
	float frequencyValue = (100.0f - clampedValue) * 219.80f + 20.0f;
	FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(lowpassHandle, FName("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find Value input"));
		return;
	}
	FAudioParameter valueParam = FAudioParameter(TEXT("Value"), frequencyValue);
	FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
	builder->SetNodeInputDefault(valueHandle, valueValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to set Value parameter"));
		return;
	}

	// Find and assign output handles directly
	firstAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutL"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find OutL output"));
		return;
	}

	if (bisStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find OutR output"));
			return;
		}
	}
}

bool UGISB_MetasoundBuilderCore::isStereo(UGisbImportContainerBase* container)
{
	if (UGisbImportContainerSimpleSound* SimpleSound = Cast<UGisbImportContainerSimpleSound>(container))
	{
		return SimpleSound->SoundWave && SimpleSound->SoundWave->NumChannels > 1;
	}
	else if (UGisbImportContainerRandom* Random = Cast<UGisbImportContainerRandom>(container))
	{
		// Determine stereo by checking any child
		for (auto& Child : Random->SoundArray)
		{
			if (isStereo(Child)) return true;
		}
		return false;
	}
	else if (UGisbImportContainerSwitch* Switch = Cast<UGisbImportContainerSwitch>(container))
	{
		// Check any child in dictionary for stereo
		for (auto& Pair : Switch->SoundDictionary)
		{
			if (isStereo(Pair.Value)) return true;
		}
		return false;
	}
	else if (UGisbImportContainerBlend* Blend = Cast<UGisbImportContainerBlend>(container))
	{
		// Check any child for stereo
		for (auto& Child : Blend->SoundArray)
		{
			if (isStereo(Child)) return true;
		}
		return false;
	}
	else
	{
		return true;
	}
}

// ============================================================================
// Core Build Logic Methods
// ============================================================================

void UGISB_MetasoundBuilderCore::BuildSimpleSoundCore(
	UMetaSoundBuilderBase* builder,
	UGisbImportContainerSimpleSound* simpleSound,
	FMetaSoundBuilderNodeOutputHandle& triggerInput,
	FMetaSoundBuilderNodeInputHandle& onFinishedInput,
	FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
	FMetaSoundBuilderNodeInputHandle* audioRightOutput,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	bool bisStereo = isStereo(simpleSound);
	
	// Setup probability filtering (modifies triggerInput and onFinishedInput)
	setupProbability(builder, simpleSound->PlaybackProbabilityPercent, triggerInput, onFinishedInput, Layout);

	// Add Wave Player node (stereo or mono depending on audio file)
	FMetaSoundNodeHandle wavePlayerHandle = builder->AddNodeByClassName(
		bisStereo ? *UGISB_MetasoundNodeLibrary::WavePlayerStereoNode : *UGISB_MetasoundNodeLibrary::WavePlayerMonoNode,
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded || !wavePlayerHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to add Wave Player node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(wavePlayerHandle, EGisbNodeCategory::SignalSource, TEXT("WavePlayer"));
	}

	// Find Wave Player input pins
	FMetaSoundBuilderNodeInputHandle waveAssetHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Wave Asset"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find Wave Asset input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle playHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Play"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find Play input"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle loopHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Loop"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find Loop input"));
		return;
	}

	// Set Wave Player parameters
	FAudioParameter waveParam = FAudioParameter(TEXT("Wave Asset"), simpleSound->SoundWave);
	FMetasoundFrontendLiteral waveValue = FMetasoundFrontendLiteral(waveParam);
	builder->SetNodeInputDefault(waveAssetHandle, waveValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to set Wave Asset"));
		return;
	}

	FAudioParameter loopParam = FAudioParameter(TEXT("Loop"), simpleSound->loop);
	FMetasoundFrontendLiteral loopValue = FMetasoundFrontendLiteral(loopParam);
	builder->SetNodeInputDefault(loopHandle, loopValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to set Loop parameter"));
		return;
	}

	// Connect trigger input to wave player
	builder->ConnectNodes(triggerInput, playHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to connect trigger to wave player"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(triggerInput.NodeID), wavePlayerHandle);
	}

	// Get Wave Player outputs
	FMetaSoundBuilderNodeOutputHandle onFinishedHandle = builder->FindNodeOutputByName(wavePlayerHandle, TEXT("On Finished"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find On Finished output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle audioLeftHandle = builder->FindNodeOutputByName(
		wavePlayerHandle,
		bisStereo ? TEXT("Out Left") : TEXT("Out Mono"),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find audio left/mono output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	if (bisStereo)
	{
		audioRightHandle = builder->FindNodeOutputByName(wavePlayerHandle, TEXT("Out Right"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to find audio right output"));
			return;
		}
	}

	// Apply audio effects chain (Volume, Pitch, Lowpass)
	// This modifies audioLeftHandle and audioRightHandle to point to processed outputs
	setupAttributes(builder, simpleSound, bisStereo, &triggerInput, audioLeftHandle, audioRightHandle, Layout);

	// Connect wave player's On Finished to the output finish handle
	builder->ConnectNodes(onFinishedHandle, onFinishedInput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to connect On Finished"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(onFinishedHandle.NodeID), FMetaSoundNodeHandle(onFinishedInput.NodeID));
	}

	// Connect final audio outputs to graph outputs (automatic connection)
	builder->ConnectNodes(audioLeftHandle, audioLeftOutput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to connect audio left output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(
			FMetaSoundNodeHandle(audioLeftHandle.NodeID),
			FMetaSoundNodeHandle(audioLeftOutput.NodeID)
		);
	}

	if (audioRightOutput && bisStereo)
	{
		builder->ConnectNodes(audioRightHandle, *audioRightOutput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundCore: FAILED to connect audio right output"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(
				FMetaSoundNodeHandle(audioRightHandle.NodeID),
				FMetaSoundNodeHandle(audioRightOutput->NodeID)
			);
		}
	}
}

void UGISB_MetasoundBuilderCore::BuildRandomCore(
	UMetaSoundBuilderBase* builder,
	UGisbImportContainerRandom* randomContainer,
	const FString& Name,
	FMetaSoundBuilderNodeOutputHandle& triggerInput,
	FMetaSoundBuilderNodeInputHandle& onFinishedInput,
	FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
	FMetaSoundBuilderNodeInputHandle* audioRightOutput,
	GisbMetasoundLayoutManager* Layout
)
{
	EMetaSoundBuilderResult result;

	// Extract parameters from container (eliminates redundant params)
	int32 avoidLastPlayed = randomContainer->AvoidLastPlayed;
	float playbackProbability = randomContainer->PlaybackProbabilityPercent;
	bool bisStereo = isStereo(randomContainer);

	// Setup probability filtering at container level
	setupProbability(builder, playbackProbability, triggerInput, onFinishedInput, Layout);

	// Build child patches using PatchBuilder's public method (uses globals)
	TArray<FChildPatchResult> childResults;
	for (int32 i = 0; i < randomContainer->SoundArray.Num(); i++)
	{
		FChildPatchResult childResult = UGISB_MetasoundPatchBuilder::BuildChildNode(
			randomContainer->SoundArray[i],
			Name,
			i
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildRandomCore: Skipping failed child %d in %s"), i, *Name);
			continue;
		}

		childResults.Add(childResult);
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: No valid children built"));
		return;
	}

	if (childResults.Num() > 8)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: >8 children not yet supported"));
		return;
	}

	// Add Random Selector node
	FMetaSoundNodeHandle randomHandle = builder->AddNode(UGISB_MetasoundNodeLibrary::GisbRandomNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !randomHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to add Random node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(randomHandle, EGisbNodeCategory::SignalProcessor, TEXT("RandomSelector"));
	}

	// Connect trigger input to random selector
	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(randomHandle, TEXT("Exec"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find Exec input"));
		return;
	}

	builder->ConnectNodes(triggerInput, execHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect trigger to random"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(triggerInput.NodeID), randomHandle);
	}

	// Configure No Repeats parameter
	FMetaSoundBuilderNodeInputHandle noRepeatHandle = builder->FindNodeInputByName(randomHandle, TEXT("No Repeats"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find No Repeats input"));
		return;
	}

	FAudioParameter noRepeatParam = FAudioParameter(TEXT("No Repeats"), avoidLastPlayed);
	FMetasoundFrontendLiteral noRepeatValue = FMetasoundFrontendLiteral(noRepeatParam);
	builder->SetNodeInputDefault(noRepeatHandle, noRepeatValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to set No Repeats"));
		return;
	}

	// Configure Possibilities parameter
	TArray<int32> possibilities;
	for (int32 i = 0; i < childResults.Num(); i++)
	{
		possibilities.Add(i);
	}

	FMetaSoundBuilderNodeInputHandle possibilitiesHandle = builder->FindNodeInputByName(randomHandle, TEXT("Possibilities"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find Possibilities input"));
		return;
	}

	FAudioParameter possibilitiesParam = FAudioParameter(TEXT("Possibilities"), possibilities);
	FMetasoundFrontendLiteral possibilitiesValue = FMetasoundFrontendLiteral(possibilitiesParam);
	builder->SetNodeInputDefault(possibilitiesHandle, possibilitiesValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to set Possibilities"));
		return;
	}

	// Add Mixer node (array indexed by numChildren-2)
	int32 adjustedIndex = childResults.Num() < 2 ? 0 : childResults.Num() - 2;

	FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
		bisStereo ? *(*UGISB_MetasoundNodeLibrary::StereoMixerNodes)[adjustedIndex] : *(*UGISB_MetasoundNodeLibrary::MonoMixerNodes)[adjustedIndex],
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !mixerHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to add Mixer node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));
	}

	// Add TriggerAny node
	FMetaSoundNodeHandle anyHandle = builder->AddNodeByClassName(
		*((*UGISB_MetasoundNodeLibrary::TriggerAnyNodes)[adjustedIndex]),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !anyHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to add TriggerAny node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(anyHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAny"));
	}

	// Instantiate child patches and connect
	for (int32 i = 0; i < childResults.Num(); i++)
	{
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
		if (result != EMetaSoundBuilderResult::Succeeded || !childPatchNode.IsSet())
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to instantiate child patch %d"), i);
			continue;
		}

		if (Layout)
		{
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d"), i);
			Layout->RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);
		}

		bool bIsChildStereo = childResults[i].bIsStereo;

		// Connect Random output i -> Child Play input
		FMetaSoundBuilderNodeOutputHandle randomOut = builder->FindNodeOutputByName(
			randomHandle,
			*FString::Printf(TEXT("Out %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find Random Out %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find child Play input %d"), i);
			continue;
		}

		builder->ConnectNodes(randomOut, childPlayInput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect random to child %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(randomHandle, childPatchNode);
		}

		// Connect Child audio left/mono -> Mixer input i
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsChildStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find child audio left %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
			mixerHandle,
			bisStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find mixer input %d"), i);
			continue;
		}

		builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect child audio to mixer %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, mixerHandle);
		}

		// If parent is stereo, connect right channel (or duplicate mono)
		if (bisStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = childAudioLeft; // Default to mono (duplicate left)

			if (bIsChildStereo)
			{
				childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				if (result != EMetaSoundBuilderResult::Succeeded)
				{
					UE_LOG(LogTemp, Warning, TEXT("BuildRandomCore: FAILED to find child audio right %d, using left"), i);
					childAudioRight = childAudioLeft;
				}
			}

			FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
				mixerHandle,
				*FString::Printf(TEXT("In %d R"), i),
				result
			);
			if (result == EMetaSoundBuilderResult::Succeeded)
			{
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}
		}

		// Connect Child OnFinished -> TriggerAny input i
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find child On Finished %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle anyIn = builder->FindNodeInputByName(
			anyHandle,
			*FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find TriggerAny input %d"), i);
			continue;
		}

		builder->ConnectNodes(childFinished, anyIn, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect child finished to any %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, anyHandle);
		}
	}

	// Get mixer and TriggerAny outputs
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle = builder->FindNodeOutputByName(
		mixerHandle,
		bisStereo ? TEXT("Out L") : TEXT("Out"),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find mixer left output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	if (bisStereo)
	{
		audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find mixer right output"));
			return;
		}
	}

	FMetaSoundBuilderNodeOutputHandle finishedHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to find TriggerAny output"));
		return;
	}

	// Apply audio effects chain to mixed output
	setupAttributes(builder, randomContainer, bisStereo, &triggerInput, audioLeftHandle, audioRightHandle, Layout);

	// Connect finished to output
	builder->ConnectNodes(finishedHandle, onFinishedInput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect finished to output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(finishedHandle.NodeID), FMetaSoundNodeHandle(onFinishedInput.NodeID));
	}

	// Connect final audio outputs to graph outputs (automatic connection)
	builder->ConnectNodes(audioLeftHandle, audioLeftOutput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect audio left output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(
			FMetaSoundNodeHandle(audioLeftHandle.NodeID),
			FMetaSoundNodeHandle(audioLeftOutput.NodeID)
		);
	}

	if (audioRightOutput && bisStereo)
	{
		builder->ConnectNodes(audioRightHandle, *audioRightOutput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomCore: FAILED to connect audio right output"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(
				FMetaSoundNodeHandle(audioRightHandle.NodeID),
				FMetaSoundNodeHandle(audioRightOutput->NodeID)
			);
		}
	}
}

void UGISB_MetasoundBuilderCore::BuildSwitchCore(
	UMetaSoundBuilderBase* builder,
	UGisbImportContainerSwitch* switchContainer,
	const FString& Name,
	FMetaSoundBuilderNodeOutputHandle& parameterInput,
	FMetaSoundBuilderNodeOutputHandle& triggerInput,
	FMetaSoundBuilderNodeInputHandle& onFinishedInput,
	FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
	FMetaSoundBuilderNodeInputHandle* audioRightOutput,
	GisbMetasoundLayoutManager* Layout
)
{
	if (!switchContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: Container is null"));
		return;
	}

	EMetaSoundBuilderResult result;

	// Extract parameters from container
	float playbackProbability = switchContainer->PlaybackProbabilityPercent;
	bool bisStereo = isStereo(switchContainer);
	FString defaultCase = switchContainer->DefaultParameterValue.ToString();

	// Apply probability filtering
	setupProbability(builder, playbackProbability, triggerInput, onFinishedInput, Layout);

	// Build child patches recursively and collect case names
	TArray<FChildPatchResult> childResults;
	TArray<FString> caseNames;

	int32 childIndex = 0;
	for (auto& Pair : switchContainer->SoundDictionary)
	{
		caseNames.Add(Pair.Key);

		FChildPatchResult childResult = UGISB_MetasoundPatchBuilder::BuildChildNode(
			Pair.Value,
			Name,
			childIndex
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildSwitchCore: Skipping failed child %s in %s"), *Pair.Key, *Name);
			childIndex++;
			continue;
		}

		childResults.Add(childResult);
		if (childResult.bIsStereo) bisStereo = true;
		childIndex++;
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: No valid children built for %s"), *Name);
		return;
	}

	if (childResults.Num() > 8)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: >8 children not yet supported for %s"), *Name);
		return;
	}

	// Add Switch Selector node (similar to random but parameter-driven)
	FMetaSoundNodeHandle switchHandle = builder->AddNode(UGISB_MetasoundNodeLibrary::GisbSwitchNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !switchHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to add Switch node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(switchHandle, EGisbNodeCategory::SignalProcessor, TEXT("SwitchSelector"));
	}

	// Connect parameter and trigger inputs
	FMetaSoundBuilderNodeInputHandle paramHandle = builder->FindNodeInputByName(switchHandle, TEXT("Switch Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find Switch Value input"));
		return;
	}

	builder->ConnectNodes(parameterInput, paramHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect parameter to switch"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(parameterInput.NodeID), switchHandle);
	}

	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(switchHandle, TEXT("Exec"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find Exec input"));
		return;
	}

	builder->ConnectNodes(triggerInput, execHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect trigger to switch"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(triggerInput.NodeID), switchHandle);
	}

	// Configure Cases parameter
	FMetaSoundBuilderNodeInputHandle casesHandle = builder->FindNodeInputByName(switchHandle, TEXT("Cases"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find Cases input"));
		return;
	}

	FAudioParameter casesParam = FAudioParameter(TEXT("Cases"), caseNames);
	FMetasoundFrontendLiteral casesValue = FMetasoundFrontendLiteral(casesParam);
	builder->SetNodeInputDefault(casesHandle, casesValue, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to set Cases"));
		return;
	}

	// Configure Default Case parameter
	FMetaSoundBuilderNodeInputHandle defaultCaseHandle = builder->FindNodeInputByName(switchHandle, TEXT("Default Case"), result);
	if (result == EMetaSoundBuilderResult::Succeeded)
	{
		FAudioParameter defaultParam = FAudioParameter(TEXT("Default Case"), defaultCase);
		FMetasoundFrontendLiteral defaultValue = FMetasoundFrontendLiteral(defaultParam);
		builder->SetNodeInputDefault(defaultCaseHandle, defaultValue, result);
	}

	// Add Mixer and TriggerAny nodes (same as Random)
	int32 adjustedIndex = childResults.Num() < 2 ? 0 : childResults.Num() - 2;

	FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
		bisStereo ? *(*UGISB_MetasoundNodeLibrary::StereoMixerNodes)[adjustedIndex] : *(*UGISB_MetasoundNodeLibrary::MonoMixerNodes)[adjustedIndex],
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !mixerHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to add Mixer node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));
	}

	FMetaSoundNodeHandle anyHandle = builder->AddNodeByClassName(
		*((*UGISB_MetasoundNodeLibrary::TriggerAnyNodes)[adjustedIndex]),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !anyHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to add TriggerAny node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(anyHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAny"));
	}

	// Instantiate child patches and connect (same pattern as Random)
	for (int32 i = 0; i < childResults.Num(); i++)
	{
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
		if (result != EMetaSoundBuilderResult::Succeeded || !childPatchNode.IsSet())
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to instantiate child patch %d"), i);
			continue;
		}

		if (Layout)
		{
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d"), i);
			Layout->RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);
		}

		bool bIsChildStereo = childResults[i].bIsStereo;

		// Connect Switch output i -> Child Play
		FMetaSoundBuilderNodeOutputHandle switchOut = builder->FindNodeOutputByName(
			switchHandle,
			*FString::Printf(TEXT("Out %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find Switch Out %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find child Play input %d"), i);
			continue;
		}

		builder->ConnectNodes(switchOut, childPlayInput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect switch to child %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(switchHandle, childPatchNode);
		}

		// Connect Child audio -> Mixer (same as Random)
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsChildStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find child audio left %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
			mixerHandle,
			bisStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find mixer input %d"), i);
			continue;
		}

		builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect child audio to mixer %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, mixerHandle);
		}

		if (bisStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = childAudioLeft;

			if (bIsChildStereo)
			{
				childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				if (result != EMetaSoundBuilderResult::Succeeded)
				{
					UE_LOG(LogTemp, Warning, TEXT("BuildSwitchCore: FAILED to find child audio right %d, using left"), i);
					childAudioRight = childAudioLeft;
				}
			}

			FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
				mixerHandle,
				*FString::Printf(TEXT("In %d R"), i),
				result
			);
			if (result == EMetaSoundBuilderResult::Succeeded)
			{
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}
		}

		// Connect Child OnFinished -> TriggerAny
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find child On Finished %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle anyIn = builder->FindNodeInputByName(
			anyHandle,
			*FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find TriggerAny input %d"), i);
			continue;
		}

		builder->ConnectNodes(childFinished, anyIn, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect child finished to any %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, anyHandle);
		}
	}

	// Get outputs (same as Random)
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle = builder->FindNodeOutputByName(
		mixerHandle,
		bisStereo ? TEXT("Out L") : TEXT("Out"),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find mixer left output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	if (bisStereo)
	{
		audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find mixer right output"));
			return;
		}
	}

	FMetaSoundBuilderNodeOutputHandle finishedHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to find TriggerAny output"));
		return;
	}

	// Connect finished to output
	builder->ConnectNodes(finishedHandle, onFinishedInput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect finished to output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(finishedHandle.NodeID), FMetaSoundNodeHandle(onFinishedInput.NodeID));
	}

	// Apply audio effects chain (Volume, Pitch, Lowpass)
	setupAttributes(builder, switchContainer, bisStereo, &triggerInput, audioLeftHandle, audioRightHandle, Layout);

	// Connect audio outputs to graph outputs
	builder->ConnectNodes(audioLeftHandle, audioLeftOutput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect audio left output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(
			FMetaSoundNodeHandle(audioLeftHandle.NodeID),
			FMetaSoundNodeHandle(audioLeftOutput.NodeID)
		);
	}

	if (audioRightOutput && bisStereo)
	{
		builder->ConnectNodes(audioRightHandle, *audioRightOutput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildSwitchCore: FAILED to connect audio right output"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(
				FMetaSoundNodeHandle(audioRightHandle.NodeID),
				FMetaSoundNodeHandle(audioRightOutput->NodeID)
			);
		}
	}
}

void UGISB_MetasoundBuilderCore::BuildBlendCore(
	UMetaSoundBuilderBase* builder,
	UGisbImportContainerBlend* blendContainer,
	const FString& Name,
	FMetaSoundBuilderNodeOutputHandle& triggerInput,
	FMetaSoundBuilderNodeInputHandle& onFinishedInput,
	FMetaSoundBuilderNodeInputHandle& audioLeftOutput,
	FMetaSoundBuilderNodeInputHandle* audioRightOutput,
	GisbMetasoundLayoutManager* Layout
)
{
	if (!blendContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: Container is null"));
		return;
	}

	EMetaSoundBuilderResult result;

	// Extract parameters from container
	float playbackProbability = blendContainer->PlaybackProbabilityPercent;
	bool bisStereo = isStereo(blendContainer);

	// Apply probability filtering
	setupProbability(builder, playbackProbability, triggerInput, onFinishedInput, Layout);

	// Build child patches recursively
	TArray<FChildPatchResult> childResults;

	for (int32 i = 0; i < blendContainer->SoundArray.Num(); i++)
	{
		FChildPatchResult childResult = UGISB_MetasoundPatchBuilder::BuildChildNode(
			blendContainer->SoundArray[i],
			Name,
			i
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildBlendCore: Skipping failed child %d in %s"), i, *Name);
			continue;
		}

		childResults.Add(childResult);
		if (childResult.bIsStereo) bisStereo = true;
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: No valid children built for %s"), *Name);
		return;
	}

	if (childResults.Num() > 8)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: >8 children not yet supported for %s"), *Name);
		return;
	}

	// Add Mixer node (array indexed by numChildren-2)
	int32 adjustedIndex = childResults.Num() < 2 ? 0 : childResults.Num() - 2;

	FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
		bisStereo ? *(*UGISB_MetasoundNodeLibrary::StereoMixerNodes)[adjustedIndex] : *(*UGISB_MetasoundNodeLibrary::MonoMixerNodes)[adjustedIndex],
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !mixerHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to add Mixer node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));
	}

	// Add TriggerAccumulate node (waits for all triggers)
	FMetaSoundNodeHandle accumulateHandle = builder->AddNodeByClassName(
		*((*UGISB_MetasoundNodeLibrary::TriggerAccumulateNodes)[childResults.Num() - 1]),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded || !accumulateHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to add TriggerAccumulate node"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterNode(accumulateHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAccumulate"));
	}

	// Instantiate child patches and connect
	for (int32 i = 0; i < childResults.Num(); i++)
	{
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
		if (result != EMetaSoundBuilderResult::Succeeded || !childPatchNode.IsSet())
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to instantiate child patch %d"), i);
			continue;
		}

		if (Layout)
		{
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d"), i);
			Layout->RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);
		}

		bool bIsChildStereo = childResults[i].bIsStereo;

		// Connect trigger input -> Child Play input (all children play simultaneously)
		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find child Play input %d"), i);
			continue;
		}

		builder->ConnectNodes(triggerInput, childPlayInput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect trigger to child %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(FMetaSoundNodeHandle(triggerInput.NodeID), childPatchNode);
		}

		// Connect Child audio left/mono -> Mixer input i
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsChildStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find child audio left %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
			mixerHandle,
			bisStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find mixer input %d"), i);
			continue;
		}

		builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect child audio to mixer %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, mixerHandle);
		}

		// If parent is stereo, connect right channel (or duplicate mono)
		if (bisStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = childAudioLeft; // Default to mono (duplicate left)

			if (bIsChildStereo)
			{
				childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				if (result != EMetaSoundBuilderResult::Succeeded)
				{
					UE_LOG(LogTemp, Warning, TEXT("BuildBlendCore: FAILED to find child audio right %d, using left"), i);
					childAudioRight = childAudioLeft;
				}
			}

			FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
				mixerHandle,
				*FString::Printf(TEXT("In %d R"), i),
				result
			);
			if (result == EMetaSoundBuilderResult::Succeeded)
			{
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}
		}

		// Connect Child OnFinished -> TriggerAccumulate input i
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find child On Finished %d"), i);
			continue;
		}

		FMetaSoundBuilderNodeInputHandle accumulateIn = builder->FindNodeInputByName(
			accumulateHandle,
			*FString::Printf(TEXT("In %d"), i),
			result
		);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find TriggerAccumulate input %d"), i);
			continue;
		}

		builder->ConnectNodes(childFinished, accumulateIn, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect child finished to accumulate %d"), i);
			continue;
		}

		if (Layout)
		{
			Layout->RegisterConnection(childPatchNode, accumulateHandle);
		}
	}

	// Get mixer and TriggerAccumulate outputs
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle = builder->FindNodeOutputByName(
		mixerHandle,
		bisStereo ? TEXT("Out L") : TEXT("Out"),
		result
	);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find mixer left output"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	if (bisStereo)
	{
		audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find mixer right output"));
			return;
		}
	}

	FMetaSoundBuilderNodeOutputHandle finishedHandle = builder->FindNodeOutputByName(accumulateHandle, TEXT("Out"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to find TriggerAccumulate output"));
		return;
	}

	// Connect finished to output
	builder->ConnectNodes(finishedHandle, onFinishedInput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect finished to output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(FMetaSoundNodeHandle(finishedHandle.NodeID), FMetaSoundNodeHandle(onFinishedInput.NodeID));
	}

	// Apply audio effects chain (Volume, Pitch, Lowpass)
	setupAttributes(builder, blendContainer, bisStereo, &triggerInput, audioLeftHandle, audioRightHandle, Layout);

	// Connect audio outputs to graph outputs
	builder->ConnectNodes(audioLeftHandle, audioLeftOutput, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect audio left output"));
		return;
	}

	if (Layout)
	{
		Layout->RegisterConnection(
			FMetaSoundNodeHandle(audioLeftHandle.NodeID),
			FMetaSoundNodeHandle(audioLeftOutput.NodeID)
		);
	}

	if (audioRightOutput && bisStereo)
	{
		builder->ConnectNodes(audioRightHandle, *audioRightOutput, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildBlendCore: FAILED to connect audio right output"));
			return;
		}

		if (Layout)
		{
			Layout->RegisterConnection(
				FMetaSoundNodeHandle(audioRightHandle.NodeID),
				FMetaSoundNodeHandle(audioRightOutput->NodeID)
			);
		}
	}
}
