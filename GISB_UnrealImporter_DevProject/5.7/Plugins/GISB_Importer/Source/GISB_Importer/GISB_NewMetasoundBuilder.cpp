// Copyright Demute SRL. All Rights Reserved.

#include "GISB_NewMetasoundBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundDocumentInterface.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundEditorSubsystem.h"
#include <cmath>

#include "GisbSoundBankDataAsset.h"
#include "GisbContainerConverter.h"

// Static member variable initializations
FMetasoundFrontendClassName* UGISB_NewMetasoundBuilder::WavePlayerMonoNode = nullptr;
FMetasoundFrontendClassName* UGISB_NewMetasoundBuilder::WavePlayerStereoNode = nullptr;
FMetasoundFrontendClassName* UGISB_NewMetasoundBuilder::ProbabilityNode = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_NewMetasoundBuilder::MonoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_NewMetasoundBuilder::StereoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_NewMetasoundBuilder::TriggerAnyNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_NewMetasoundBuilder::TriggerAccumulateNodes = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::AudioRerouteNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbRandomNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbSwitchNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbVolumeNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbPitchNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbLowpassNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::GisbAttenuationNode = nullptr;

// Global variables
FString PathGlobal;
UMetaSoundBuilderSubsystem* BuilderGlobal;

void UGISB_NewMetasoundBuilder::TestBuilder(UGisbSoundBankDataAsset* dataAsset)
{
	if (!dataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset is null!"));
		return;
	}

	UGisbContainerBase* RootContainer = dataAsset->RootContainer;
	if (!RootContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("RootContainer is null!"));
		return;
	}

	// Convert runtime container to import container using the converter
	UGisbImportContainerBase* ImportContainer = UGisbContainerConverter::RuntimeToImportContainer(
		RootContainer,
		GetTransientPackage()
	);

	if (!ImportContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert runtime container to import container"));
		return;
	}
	
	BuilderGlobal = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
	PathGlobal = "/Game/A_TESTING/Metasounds";
	SetupNodes();
	
	UGISB_NewMetasoundBuilder::BuildChildNode(ImportContainer, "Testing", 0);

}

void UGISB_NewMetasoundBuilder::SetupNodes()
{
	FName ue = TEXT("UE");
	FName wave = TEXT("Wave Player");

	UMetaSoundPatch* ReroutePatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_AudioReroute.GISB_AudioReroute")));
	AudioRerouteNode = TScriptInterface<IMetaSoundDocumentInterface>(ReroutePatch);

	WavePlayerMonoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Mono"));

	WavePlayerStereoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Stereo"));

	UMetaSoundPatch* AttenuationPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_Attenuation.GISB_Attenuation")));
	GisbAttenuationNode = TScriptInterface<IMetaSoundDocumentInterface>(AttenuationPatch);

	ProbabilityNode = new FMetasoundFrontendClassName(ue, TEXT("Trigger Filter"));

	UMetaSoundPatch* lowpassPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomLowpass.GISB_RandomLowpass")));
	GisbLowpassNode = TScriptInterface<IMetaSoundDocumentInterface>(lowpassPatch);

	UMetaSoundPatch* pitchPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPitch.GISB_RandomPitch")));
	GisbPitchNode = TScriptInterface<IMetaSoundDocumentInterface>(pitchPatch);

	UMetaSoundPatch* randomPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPlayer.GISB_RandomPlayer")));
	GisbRandomNode = TScriptInterface<IMetaSoundDocumentInterface>(randomPatch);

	UMetaSoundPatch* volumePatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomVolume.GISB_RandomVolume")));
	GisbVolumeNode = TScriptInterface<IMetaSoundDocumentInterface>(volumePatch);

	UMetaSoundPatch* switchPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_SwitchPlayer.GISB_SwitchPlayer")));
	GisbSwitchNode = TScriptInterface<IMetaSoundDocumentInterface>(switchPatch);

	FName mixer = TEXT("AudioMixer");
	FName any = TEXT("TriggerAny");
	FName accumulate = TEXT("TriggerAccumulator");

	MonoMixerNodes = new TArray<FMetasoundFrontendClassName*>;
	StereoMixerNodes = new TArray<FMetasoundFrontendClassName*>;
	TriggerAnyNodes = new TArray<FMetasoundFrontendClassName*>;
	TriggerAccumulateNodes = new TArray<FMetasoundFrontendClassName*>;

	TriggerAccumulateNodes->Add(new FMetasoundFrontendClassName(accumulate, TEXT("Trigger Accumulate (1)")));

	for (int i = 2; i <= 8; i++) //MS multi-input/output nodes start from 2 and end at 8    // Except for Trigger Accumulate which is 1
	{
        FName monoName = FName(*FString::Printf(TEXT("Audio Mixer (Mono, %d)"), i));
		FName stereoName = FName(*FString::Printf(TEXT("Audio Mixer (Stereo, %d)"), i));
		FName anyName = FName(*FString::Printf(TEXT("Trigger Any (%d)"), i));
		FName accumulateName = FName(*FString::Printf(TEXT("Trigger Accumulate (%d)"), i));

		FMetasoundFrontendClassName* monoNode = new FMetasoundFrontendClassName(mixer, monoName);
		FMetasoundFrontendClassName* stereoNode = new FMetasoundFrontendClassName(mixer, stereoName);
		FMetasoundFrontendClassName* triggerAnyNode = new FMetasoundFrontendClassName(any, anyName);
		FMetasoundFrontendClassName* triggerAccumulateNode = new FMetasoundFrontendClassName(accumulate, accumulateName);

		MonoMixerNodes->Add(monoNode);
		StereoMixerNodes->Add(stereoNode);
		TriggerAnyNodes->Add(triggerAnyNode);
		TriggerAccumulateNodes->Add(triggerAccumulateNode);
	}
}

TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::BuildSimpleSoundNode(UGisbImportContainerSimpleSound* simpleSound, const FString& Name)
{
	EMetaSoundBuilderResult result;

	// Create Patch Builder (not Source Builder!)
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(FName(Name), result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	  return nullptr;

	// Create layout manager for automatic node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Determine if stereo
	bool isStereo = simpleSound->SoundWave && simpleSound->SoundWave->NumChannels > 1;

	// Add Graph INPUT nodes (these become the patch's inputs when used as a node)
	FMetaSoundBuilderNodeOutputHandle triggerInputHandle = builder->AddGraphInputNode(
	  FName("Play"),
	  FName("Trigger"),
	  FMetasoundFrontendLiteral(),
	  result
	);
	FMetaSoundNodeHandle triggerInputNode(triggerInputHandle.NodeID);
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));

	// Add Graph OUTPUT nodes (these become the patch's outputs when used as a node)
	FMetaSoundBuilderNodeInputHandle onFinishedOutputHandle = builder->AddGraphOutputNode(
	  FName("On Finished"),
	  FName("Trigger"),
	  FMetasoundFrontendLiteral(),
	  result
	);
	FMetaSoundNodeHandle onFinishedOutputNode(onFinishedOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(onFinishedOutputNode, FName("On Finished"));

	setupProbability(builder, simpleSound->PlaybackProbabilityPercent, triggerInputHandle, onFinishedOutputHandle, &Layout);

	//ConnectProbabilityNode(builder, simpleSound->PlaybackProbabilityPercent, triggerInputHandle, onFinishedOutputHandle);

	// Add Wave Player node
	FMetaSoundNodeHandle wavePlayerHandle = builder->AddNodeByClassName(
	  isStereo ? *WavePlayerStereoNode : *WavePlayerMonoNode,
	  result
	);
	Layout.RegisterNode(wavePlayerHandle, EGisbNodeCategory::SignalSource, TEXT("WavePlayer"));

	// Set Wave Player parameters
	FMetaSoundBuilderNodeInputHandle waveAssetHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Wave Asset"), result);
	FMetaSoundBuilderNodeInputHandle playHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Play"), result);
	FMetaSoundBuilderNodeInputHandle loopHandle = builder->FindNodeInputByName(wavePlayerHandle, TEXT("Loop"), result);

	FAudioParameter waveParam = FAudioParameter(TEXT("Wave Asset"), simpleSound->SoundWave);
	builder->SetNodeInputDefault(waveAssetHandle, FMetasoundFrontendLiteral(waveParam), result);

	FAudioParameter loopParam = FAudioParameter(TEXT("Loop"), simpleSound->loop);
	builder->SetNodeInputDefault(loopHandle, FMetasoundFrontendLiteral(loopParam), result);

	// Connect trigger input to wave player
	builder->ConnectNodes(triggerInputHandle, playHandle, result);
	Layout.RegisterConnection(triggerInputNode, wavePlayerHandle);

	// Get Wave Player outputs
	FMetaSoundBuilderNodeOutputHandle onFinishedHandle = builder->FindNodeOutputByName(wavePlayerHandle, TEXT("On Finished"), result);
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle = builder->FindNodeOutputByName(
	  wavePlayerHandle,
	  isStereo ? TEXT("Out Left") : TEXT("Out Mono"),
	  result
	);

	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	if (isStereo)
	{
	  audioRightHandle = builder->FindNodeOutputByName(wavePlayerHandle, TEXT("Out Right"), result);
	}

	// Apply audio effects chain (updates audio handles)
	setupAttributes(builder, simpleSound, isStereo, &triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);

	FMetaSoundBuilderNodeInputHandle audioLeftOutputHandle = builder->AddGraphOutputNode(
	  isStereo ? FName("Audio Left") : FName("Audio Mono"),
	  FName("Audio"),
	  FMetasoundFrontendLiteral(),
	  result
	);
	FMetaSoundNodeHandle audioLeftOutputNode(audioLeftOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(audioLeftOutputNode, isStereo ? FName("Audio Left") : FName("Audio Mono"));

	// Connect to outputs
	builder->ConnectNodes(onFinishedHandle, onFinishedOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(onFinishedHandle.NodeID), onFinishedOutputNode);

	builder->ConnectNodes(audioLeftHandle, audioLeftOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(audioLeftHandle.NodeID), audioLeftOutputNode);

	if (isStereo)
	{
	  FMetaSoundBuilderNodeInputHandle audioRightOutputHandle = builder->AddGraphOutputNode(
	      FName("Audio Right"),
	      FName("Audio"),
	      FMetasoundFrontendLiteral(),
	      result
	  );
	  FMetaSoundNodeHandle audioRightOutputNode(audioRightOutputHandle.NodeID);
	  Layout.RegisterGraphOutputNode(audioRightOutputNode, FName("Audio Right"));

	  builder->ConnectNodes(audioRightHandle, audioRightOutputHandle, result);
	  Layout.RegisterConnection(FMetaSoundNodeHandle(audioRightHandle.NodeID), audioRightOutputNode);
	}

	// Compute and apply automatic node layout
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	// Build to asset and return
	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully built MetaSound for: %s"), *Name);
	return patch;
}


void UGISB_NewMetasoundBuilder::setupProbability(UMetaSoundPatchBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& playTrigger, FMetaSoundBuilderNodeInputHandle& onFinished, GisbMetasoundLayoutManager* Layout)
{
	if (probability > 99.99f)
	{
		return;
	}

	ConnectProbabilityNode(builder, probability, playTrigger, onFinished, Layout);
	
	//CREATING VARIABLES CURRENTLY DOES NOT WORK, IT MAKES THE PATCH CRASH

	/*EMetaSoundBuilderResult result;
	
	builder->AddGraphVariable(FName("ProbabilityPlay"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add ProbabilityPlay variable"));
		return;
	}

	builder->AddGraphVariable(FName("ProbabilityFinished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add ProbabilityFinished variable"));
		return;
	}

	// Create Set node for ProbabilityPlay
	FMetaSoundNodeHandle setPlay = builder->AddGraphVariableSetNode(FName("ProbabilityPlay"), result);
	if (result != EMetaSoundBuilderResult::Succeeded || !setPlay.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add ProbabilityPlay set node"));
		return;
	}

	FMetaSoundBuilderNodeInputHandle setPlayHandle = builder->FindNodeInputByName(setPlay, TEXT("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to find Value input on ProbabilityPlay set node"));
		return;
	}

	builder->ConnectNodes(playTrigger, setPlayHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to connect playTrigger to set node"));
		return;
	}

	// Create Get node for ProbabilityFinished
	FMetaSoundNodeHandle getFinished = builder->AddGraphVariableGetNode(FName("ProbabilityFinished"), result);
	if (result != EMetaSoundBuilderResult::Succeeded || !getFinished.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add ProbabilityFinished get node"));
		return;
	}

	FMetaSoundBuilderNodeOutputHandle getFinishedHandle = builder->FindNodeOutputByName(getFinished, TEXT("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to find Value output on get node"));
		return;
	}

	builder->ConnectNodes(getFinishedHandle, onFinished, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to connect get node to onFinished"));
		return;
	}

	// Create Get node for ProbabilityPlay (this will be the new playTrigger output)
	FMetaSoundNodeHandle getPlay = builder->AddGraphVariableGetNode(FName("ProbabilityPlay"), result);
	if (result != EMetaSoundBuilderResult::Succeeded || !getPlay.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add second ProbabilityPlay get node"));
		return;
	}

	playTrigger = builder->FindNodeOutputByName(getPlay, FName("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to find Value output on second get node"));
		return;
	}

	// Create Set node for ProbabilityFinished (this will be the new onFinished input)
	FMetaSoundNodeHandle setFinished = builder->AddGraphVariableSetNode(FName("ProbabilityFinished"), result);
	if (result != EMetaSoundBuilderResult::Succeeded || !setFinished.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to add ProbabilityFinished set node"));
		return;
	}

	onFinished = builder->FindNodeInputByName(setFinished, FName("Value"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("setupProbability: FAILED to find Value input on set node"));
		return;
	}*/
}

void UGISB_NewMetasoundBuilder::ConnectProbabilityNode(UMetaSoundPatchBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& executionHandle, FMetaSoundBuilderNodeInputHandle& finishHandle, GisbMetasoundLayoutManager* Layout)
{
	EMetaSoundBuilderResult result;

	// Add Trigger Filter (Probability) node
	FMetaSoundNodeHandle probabilityHandle;
	probabilityHandle = builder->AddNodeByClassName(*ProbabilityNode, result);
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
	anyHandle = builder->AddNodeByClassName(*((*TriggerAnyNodes)[0]), result);
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

void UGISB_NewMetasoundBuilder::ConnectVolume(UMetaSoundPatchBuilder* builder, const FGisbVolume volume, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, GisbMetasoundLayoutManager* Layout)
{
	EMetaSoundBuilderResult result;

	// Add Volume node
	FMetaSoundNodeHandle volumeHandle = builder->AddNode(GisbVolumeNode, result);
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

	if (isStereo)
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

	if (isStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to find OutR output"));
			return;
		}
	}
}

void UGISB_NewMetasoundBuilder::ConnectPitch(UMetaSoundPatchBuilder* builder, const FGisbPitch pitch, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, GisbMetasoundLayoutManager* Layout)
{
	EMetaSoundBuilderResult result;

	// Add Pitch node
	FMetaSoundNodeHandle pitchHandle = builder->AddNode(GisbPitchNode, result);
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

	if (isStereo)
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

	if (isStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to find OutR output"));
			return;
		}
	}
}

void UGISB_NewMetasoundBuilder::ConnectLowpass(UMetaSoundPatchBuilder* builder, const FGisbLowPass lowpass, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, GisbMetasoundLayoutManager* Layout)
{
	EMetaSoundBuilderResult result;

	// Add Lowpass node
	FMetaSoundNodeHandle lowpassHandle = builder->AddNode(GisbLowpassNode, result);
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

	if (isStereo)
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

	if (isStereo)
	{
		secondAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutR"), result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to find OutR output"));
			return;
		}
	}
}

void UGISB_NewMetasoundBuilder::setupAttributes(UMetaSoundPatchBuilder* builder, UGisbImportContainerSimpleSound* simpleSound, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, GisbMetasoundLayoutManager* Layout)
{
	// Apply Volume effect if configured
	if (simpleSound->VolumeDB.randomize || simpleSound->VolumeDB.value != 0)
	{
		ConnectVolume(builder, simpleSound->VolumeDB, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}

	// Apply Pitch effect if configured
	if (simpleSound->Pitch.randomize || simpleSound->Pitch.value != 0)
	{
		ConnectPitch(builder, simpleSound->Pitch, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}

	// Apply Lowpass effect if configured
	if (simpleSound->Lowpass.randomize || simpleSound->Lowpass.value != 0)
	{
		ConnectLowpass(builder, simpleSound->Lowpass, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, Layout);
	}
}

// ============================================================================
// BuildChildNode - Recursive child patch builder with stereo detection
// ============================================================================

FChildPatchResult UGISB_NewMetasoundBuilder::BuildChildNode(
	UGisbImportContainerBase* container,
	const FString& ParentName,
	int32 ChildIndex
)
{
	FChildPatchResult Result;

	if (!container)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Container is null"));
		return Result;
	}

	// Dispatch based on container type
	if (UGisbImportContainerSimpleSound* SimpleSound = Cast<UGisbImportContainerSimpleSound>(container))
	{
		// Use wav file name for SimpleSound containers
		FString WavName = SimpleSound->SoundWave ? SimpleSound->SoundWave->GetName() : FString::Printf(TEXT("SimpleSound%d"), ChildIndex);
		FString ChildName = FString::Printf(TEXT("%s_%s"), *ParentName, *WavName);

		// Determine stereo from sound wave
		Result.bIsStereo = SimpleSound->SoundWave && SimpleSound->SoundWave->NumChannels > 1;
		Result.Patch = BuildSimpleSoundNode(SimpleSound, ChildName);
	}
	else if (UGisbImportContainerRandom* Random = Cast<UGisbImportContainerRandom>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Random%d"), *ParentName, ChildIndex);

		// Build Random patch (will recursively build children)
		Result.Patch = BuildRandomNode(Random, ChildName);

		// Determine stereo by checking any child
		for (auto& Child : Random->SoundArray)
		{
			if (UGisbImportContainerSimpleSound* SimpleChild = Cast<UGisbImportContainerSimpleSound>(Child))
			{
				if (SimpleChild->SoundWave && SimpleChild->SoundWave->NumChannels > 1)
				{
					Result.bIsStereo = true;
					break;
				}
			}
			else
			{
				// Nested container - conservatively assume stereo
				Result.bIsStereo = true;
				break;
			}
		}
	}
	else if (UGisbImportContainerSwitch* Switch = Cast<UGisbImportContainerSwitch>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Switch%d"), *ParentName, ChildIndex);

		Result.Patch = BuildSwitchNode(Switch, ChildName);

		// Check any child in dictionary for stereo
		for (auto& Pair : Switch->SoundDictionary)
		{
			if (UGisbImportContainerSimpleSound* SimpleChild = Cast<UGisbImportContainerSimpleSound>(Pair.Value))
			{
				if (SimpleChild->SoundWave && SimpleChild->SoundWave->NumChannels > 1)
				{
					Result.bIsStereo = true;
					break;
				}
			}
			else
			{
				Result.bIsStereo = true;
				break;
			}
		}
	}
	else if (UGisbImportContainerBlend* Blend = Cast<UGisbImportContainerBlend>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Blend%d"), *ParentName, ChildIndex);

		Result.Patch = BuildBlendNode(Blend, ChildName);

		// Check any child for stereo
		for (auto& Child : Blend->SoundArray)
		{
			if (UGisbImportContainerSimpleSound* SimpleChild = Cast<UGisbImportContainerSimpleSound>(Child))
			{
				if (SimpleChild->SoundWave && SimpleChild->SoundWave->NumChannels > 1)
				{
					Result.bIsStereo = true;
					break;
				}
			}
			else
			{
				Result.bIsStereo = true;
				break;
			}
		}
	}
	else
	{
		FString ChildName = FString::Printf(TEXT("%s_Unknown%d"), *ParentName, ChildIndex);
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Unknown container type for %s"), *ChildName);
	}

	if (!Result.Patch)
	{
		// Note: ChildName is scoped to each case, so we recreate it for error log
		FString ChildName = FString::Printf(TEXT("%s_Failed%d"), *ParentName, ChildIndex);
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Failed to build patch for %s"), *ChildName);
	}

	return Result;
}

// ============================================================================
// BuildRandomNode - Build MetaSound patch for Random container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::BuildRandomNode(
	UGisbImportContainerRandom* randomContainer,
	const FString& Name
)
{
	if (!randomContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Stage 1: Create Patch Builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(FName(Name), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Stage 2: Initialize Layout Manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Stage 3: Add Graph Inputs/Outputs
	FMetaSoundBuilderNodeOutputHandle triggerInputHandle = builder->AddGraphInputNode(
		FName("Play"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle triggerInputNode(triggerInputHandle.NodeID);
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));

	FMetaSoundBuilderNodeInputHandle onFinishedOutputHandle = builder->AddGraphOutputNode(
		FName("On Finished"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle onFinishedOutputNode(onFinishedOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(onFinishedOutputNode, FName("On Finished"));

	// Stage 4: Setup Probability
	setupProbability(builder, randomContainer->PlaybackProbabilityPercent,
		triggerInputHandle, onFinishedOutputHandle, &Layout);

	// Stage 5: Add Core Processing
	int32 numChildren = randomContainer->SoundArray.Num();

	if (numChildren < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: No children in random container %s"), *Name);
		return nullptr;
	}

	// Build child patches recursively
	TArray<FChildPatchResult> childResults;
	bool bIsStereo = false;

	for (int32 i = 0; i < numChildren; i++)
	{
		FChildPatchResult childResult = BuildChildNode(
			randomContainer->SoundArray[i],
			Name,
			i
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildRandomNode: Skipping failed child %d in %s"), i, *Name);
			continue;
		}

		childResults.Add(childResult);
		if (childResult.bIsStereo) bIsStereo = true;
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: No valid children built for %s"), *Name);
		return nullptr;
	}

	// Declare audio output handles (populated below)
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle;
	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	FMetaSoundBuilderNodeOutputHandle onFinishedHandle;

	// CASE 1: Single child - use AudioReroute
	if (childResults.Num() == 1)
	{
		FMetaSoundNodeHandle rerouteHandle = builder->AddNode(AudioRerouteNode, result);
		Layout.RegisterNode(rerouteHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioReroute"));

		// Instantiate child patch as node
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[0].Patch, result);
		Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, TEXT("ChildPatch0"));

		// Connect trigger: Play -> Child Play
		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		builder->ConnectNodes(triggerInputHandle, childPlayInput, result);
		Layout.RegisterConnection(triggerInputNode, childPatchNode);

		// Connect audio: Child -> Reroute
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		FMetaSoundBuilderNodeInputHandle rerouteAudioInL = builder->FindNodeInputByName(rerouteHandle, TEXT("InL"), result);
		builder->ConnectNodes(childAudioLeft, rerouteAudioInL, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		if (bIsStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
			FMetaSoundBuilderNodeInputHandle rerouteAudioInR = builder->FindNodeInputByName(rerouteHandle, TEXT("InR"), result);
			builder->ConnectNodes(childAudioRight, rerouteAudioInR, result);
		}

		// Connect finish trigger: Child OnFinished -> Reroute
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		FMetaSoundBuilderNodeInputHandle rerouteExecIn = builder->FindNodeInputByName(rerouteHandle, TEXT("InExec"), result);
		builder->ConnectNodes(childFinished, rerouteExecIn, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		// Get reroute outputs
		audioLeftHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutL"), result);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutR"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutExec"), result);
	}
	// CASE 2: Multiple children (2-8) - use GisbRandomNode + Mixer + TriggerAny
	else if (childResults.Num() <= 8)
	{
		// Add GisbRandomNode for selection
		FMetaSoundNodeHandle randomHandle = builder->AddNode(GisbRandomNode, result);
		Layout.RegisterNode(randomHandle, EGisbNodeCategory::SignalProcessor, TEXT("RandomSelector"));

		// Configure GisbRandomNode
		FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(randomHandle, TEXT("Exec"), result);
		builder->ConnectNodes(triggerInputHandle, execHandle, result);
		Layout.RegisterConnection(triggerInputNode, randomHandle);

		FMetaSoundBuilderNodeInputHandle noRepeatHandle = builder->FindNodeInputByName(randomHandle, TEXT("No Repeats"), result);
		FAudioParameter noRepeatParam = FAudioParameter(TEXT("No Repeats"), randomContainer->AvoidLastPlayed);
		builder->SetNodeInputDefault(noRepeatHandle, FMetasoundFrontendLiteral(noRepeatParam), result);

		TArray<int32> possibilities;
		for (int32 i = 0; i < childResults.Num(); i++) possibilities.Add(i);
		FMetaSoundBuilderNodeInputHandle possibilitiesHandle = builder->FindNodeInputByName(randomHandle, TEXT("Possibilities"), result);
		FAudioParameter possibilitiesParam = FAudioParameter(TEXT("Possibilities"), possibilities);
		builder->SetNodeInputDefault(possibilitiesHandle, FMetasoundFrontendLiteral(possibilitiesParam), result);

		// Add mixer node (array indexed by numChildren-2)
		FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
			bIsStereo ? *(*StereoMixerNodes)[childResults.Num() - 2] : *(*MonoMixerNodes)[childResults.Num() - 2],
			result
		);
		Layout.RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));

		// Add TriggerAny node (first trigger wins)
		FMetaSoundNodeHandle anyHandle = builder->AddNodeByClassName(
			*((*TriggerAnyNodes)[childResults.Num() - 2]),
			result
		);
		Layout.RegisterNode(anyHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAny"));

		// Instantiate child patches and connect
		for (int32 i = 0; i < childResults.Num(); i++)
		{
			FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d"), i);
			Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);

			// Connect Random output i -> Child Play
			FMetaSoundBuilderNodeOutputHandle randomOut = builder->FindNodeOutputByName(
				randomHandle,
				*FString::Printf(TEXT("Out %d"), i),
				result
			);
			FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
			builder->ConnectNodes(randomOut, childPlayInput, result);
			Layout.RegisterConnection(randomHandle, childPatchNode);

			// Connect Child audio -> Mixer input i
			FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
				childPatchNode,
				bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
				result
			);
			FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
				mixerHandle,
				bIsStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
			Layout.RegisterConnection(childPatchNode, mixerHandle);

			if (bIsStereo)
			{
				FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
					mixerHandle,
					*FString::Printf(TEXT("In %d R"), i),
					result
				);
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}

			// Connect Child OnFinished -> TriggerAny input i
			FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
			FMetaSoundBuilderNodeInputHandle anyIn = builder->FindNodeInputByName(
				anyHandle,
				*FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childFinished, anyIn, result);
			Layout.RegisterConnection(childPatchNode, anyHandle);
		}

		// Get mixer/any outputs
		audioLeftHandle = builder->FindNodeOutputByName(
			mixerHandle,
			bIsStereo ? TEXT("Out L") : TEXT("Out"),
			result
		);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	}
	else
	{
		// CASE 3: >8 children - hierarchical cascading (TODO)
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: >8 children not yet implemented for %s"), *Name);
		return nullptr;
	}

	// Stage 6: Apply Audio Effects Chain
	if (randomContainer->VolumeDB.randomize || randomContainer->VolumeDB.value != 0)
	{
		ConnectVolume(builder, randomContainer->VolumeDB, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (randomContainer->Pitch.randomize || randomContainer->Pitch.value != 0)
	{
		ConnectPitch(builder, randomContainer->Pitch, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (randomContainer->Lowpass.randomize || randomContainer->Lowpass.value != 0)
	{
		ConnectLowpass(builder, randomContainer->Lowpass, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	// Stage 7: Connect Graph Outputs
	FMetaSoundBuilderNodeInputHandle audioLeftOutputHandle = builder->AddGraphOutputNode(
		bIsStereo ? FName("Audio Left") : FName("Audio Mono"),
		FName("Audio"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle audioLeftOutputNode(audioLeftOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(audioLeftOutputNode, bIsStereo ? FName("Audio Left") : FName("Audio Mono"));

	builder->ConnectNodes(audioLeftHandle, audioLeftOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(audioLeftHandle.NodeID), audioLeftOutputNode);

	if (bIsStereo)
	{
		FMetaSoundBuilderNodeInputHandle audioRightOutputHandle = builder->AddGraphOutputNode(
			FName("Audio Right"),
			FName("Audio"),
			FMetasoundFrontendLiteral(),
			result
		);
		FMetaSoundNodeHandle audioRightOutputNode(audioRightOutputHandle.NodeID);
		Layout.RegisterGraphOutputNode(audioRightOutputNode, FName("Audio Right"));

		builder->ConnectNodes(audioRightHandle, audioRightOutputHandle, result);
		Layout.RegisterConnection(FMetaSoundNodeHandle(audioRightHandle.NodeID), audioRightOutputNode);
	}

	builder->ConnectNodes(onFinishedHandle, onFinishedOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(onFinishedHandle.NodeID), onFinishedOutputNode);

	// Stage 8: Compute Layout & Build Asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(
		builder,
		"ISX - Demute",
		Name,
		PathGlobal,
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildRandomNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// BuildBlendNode - Build MetaSound patch for Blend container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::BuildBlendNode(
	UGisbImportContainerBlend* blendContainer,
	const FString& Name
)
{
	if (!blendContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Stage 1: Create Patch Builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(FName(Name), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Stage 2: Initialize Layout Manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Stage 3: Add Graph Inputs/Outputs
	FMetaSoundBuilderNodeOutputHandle triggerInputHandle = builder->AddGraphInputNode(
		FName("Play"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle triggerInputNode(triggerInputHandle.NodeID);
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));

	FMetaSoundBuilderNodeInputHandle onFinishedOutputHandle = builder->AddGraphOutputNode(
		FName("On Finished"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle onFinishedOutputNode(onFinishedOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(onFinishedOutputNode, FName("On Finished"));

	// Stage 4: Setup Probability
	setupProbability(builder, blendContainer->PlaybackProbabilityPercent,
		triggerInputHandle, onFinishedOutputHandle, &Layout);

	// Stage 5: Add Core Processing
	int32 numChildren = blendContainer->SoundArray.Num();

	if (numChildren < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: No children in blend container %s"), *Name);
		return nullptr;
	}

	// Build child patches recursively
	TArray<FChildPatchResult> childResults;
	bool bIsStereo = false;

	for (int32 i = 0; i < numChildren; i++)
	{
		FChildPatchResult childResult = BuildChildNode(
			blendContainer->SoundArray[i],
			Name,
			i
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildBlendNode: Skipping failed child %d in %s"), i, *Name);
			continue;
		}

		childResults.Add(childResult);
		if (childResult.bIsStereo) bIsStereo = true;
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: No valid children built for %s"), *Name);
		return nullptr;
	}

	// Declare audio output handles (populated below)
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle;
	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	FMetaSoundBuilderNodeOutputHandle onFinishedHandle;

	// CASE 1: Single child - use AudioReroute
	if (childResults.Num() == 1)
	{
		FMetaSoundNodeHandle rerouteHandle = builder->AddNode(AudioRerouteNode, result);
		Layout.RegisterNode(rerouteHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioReroute"));

		// Instantiate child patch as node
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[0].Patch, result);
		Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, TEXT("ChildPatch0"));

		// Connect trigger: Play -> Child Play
		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		builder->ConnectNodes(triggerInputHandle, childPlayInput, result);
		Layout.RegisterConnection(triggerInputNode, childPatchNode);

		// Connect audio: Child -> Reroute
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		FMetaSoundBuilderNodeInputHandle rerouteAudioInL = builder->FindNodeInputByName(rerouteHandle, TEXT("InL"), result);
		builder->ConnectNodes(childAudioLeft, rerouteAudioInL, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		if (bIsStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
			FMetaSoundBuilderNodeInputHandle rerouteAudioInR = builder->FindNodeInputByName(rerouteHandle, TEXT("InR"), result);
			builder->ConnectNodes(childAudioRight, rerouteAudioInR, result);
		}

		// Connect finish trigger: Child OnFinished -> Reroute
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		FMetaSoundBuilderNodeInputHandle rerouteExecIn = builder->FindNodeInputByName(rerouteHandle, TEXT("InExec"), result);
		builder->ConnectNodes(childFinished, rerouteExecIn, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		// Get reroute outputs
		audioLeftHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutL"), result);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutR"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutExec"), result);
	}
	// CASE 2: Multiple children (2-8) - use Mixer + TriggerAccumulate (wait for all)
	else if (childResults.Num() <= 8)
	{
		// Add mixer node (array indexed by numChildren-2)
		FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
			bIsStereo ? *(*StereoMixerNodes)[childResults.Num() - 2] : *(*MonoMixerNodes)[childResults.Num() - 2],
			result
		);
		Layout.RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));

		// Add TriggerAccumulate node (wait for all children)
		FMetaSoundNodeHandle accumulateHandle = builder->AddNodeByClassName(
			*((*TriggerAccumulateNodes)[childResults.Num() - 1]), // -1 because array starts at 1 input
			result
		);
		Layout.RegisterNode(accumulateHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAccumulate"));

		// Instantiate child patches and connect
		for (int32 i = 0; i < childResults.Num(); i++)
		{
			FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d"), i);
			Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);

			// Connect Play trigger -> Child Play (ALL children triggered simultaneously)
			FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
			builder->ConnectNodes(triggerInputHandle, childPlayInput, result);
			Layout.RegisterConnection(triggerInputNode, childPatchNode);

			// Connect Child audio -> Mixer input i
			FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
				childPatchNode,
				bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
				result
			);
			FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
				mixerHandle,
				bIsStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
			Layout.RegisterConnection(childPatchNode, mixerHandle);

			if (bIsStereo)
			{
				FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
					mixerHandle,
					*FString::Printf(TEXT("In %d R"), i),
					result
				);
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}

			// Connect Child OnFinished -> TriggerAccumulate input i
			FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
			FMetaSoundBuilderNodeInputHandle accumulateIn = builder->FindNodeInputByName(
				accumulateHandle,
				*FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childFinished, accumulateIn, result);
			Layout.RegisterConnection(childPatchNode, accumulateHandle);
		}

		// Get mixer/accumulate outputs
		audioLeftHandle = builder->FindNodeOutputByName(
			mixerHandle,
			bIsStereo ? TEXT("Out L") : TEXT("Out"),
			result
		);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(accumulateHandle, TEXT("Out"), result);
	}
	else
	{
		// CASE 3: >8 children - hierarchical cascading (TODO)
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: >8 children not yet implemented for %s"), *Name);
		return nullptr;
	}

	// Stage 6: Apply Audio Effects Chain
	if (blendContainer->VolumeDB.randomize || blendContainer->VolumeDB.value != 0)
	{
		ConnectVolume(builder, blendContainer->VolumeDB, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (blendContainer->Pitch.randomize || blendContainer->Pitch.value != 0)
	{
		ConnectPitch(builder, blendContainer->Pitch, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (blendContainer->Lowpass.randomize || blendContainer->Lowpass.value != 0)
	{
		ConnectLowpass(builder, blendContainer->Lowpass, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	// Stage 7: Connect Graph Outputs
	FMetaSoundBuilderNodeInputHandle audioLeftOutputHandle = builder->AddGraphOutputNode(
		bIsStereo ? FName("Audio Left") : FName("Audio Mono"),
		FName("Audio"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle audioLeftOutputNode(audioLeftOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(audioLeftOutputNode, bIsStereo ? FName("Audio Left") : FName("Audio Mono"));

	builder->ConnectNodes(audioLeftHandle, audioLeftOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(audioLeftHandle.NodeID), audioLeftOutputNode);

	if (bIsStereo)
	{
		FMetaSoundBuilderNodeInputHandle audioRightOutputHandle = builder->AddGraphOutputNode(
			FName("Audio Right"),
			FName("Audio"),
			FMetasoundFrontendLiteral(),
			result
		);
		FMetaSoundNodeHandle audioRightOutputNode(audioRightOutputHandle.NodeID);
		Layout.RegisterGraphOutputNode(audioRightOutputNode, FName("Audio Right"));

		builder->ConnectNodes(audioRightHandle, audioRightOutputHandle, result);
		Layout.RegisterConnection(FMetaSoundNodeHandle(audioRightHandle.NodeID), audioRightOutputNode);
	}

	builder->ConnectNodes(onFinishedHandle, onFinishedOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(onFinishedHandle.NodeID), onFinishedOutputNode);

	// Stage 8: Compute Layout & Build Asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(
		builder,
		"ISX - Demute",
		Name,
		PathGlobal,
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildBlendNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// BuildSwitchNode - Build MetaSound patch for Switch container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_NewMetasoundBuilder::BuildSwitchNode(
	UGisbImportContainerSwitch* switchContainer,
	const FString& Name
)
{
	if (!switchContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Stage 1: Create Patch Builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(FName(Name), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Stage 2: Initialize Layout Manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Stage 3: Add Graph Inputs/Outputs (including Switch Parameter)
	FMetaSoundBuilderNodeOutputHandle triggerInputHandle = builder->AddGraphInputNode(
		FName("Play"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle triggerInputNode(triggerInputHandle.NodeID);
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));

	// Add Switch Parameter input (String type)
	FMetaSoundBuilderNodeOutputHandle parameterInputHandle = builder->AddGraphInputNode(
		FName("Switch Parameter"),
		FName("String"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle parameterInputNode(parameterInputHandle.NodeID);
	Layout.RegisterGraphInputNode(parameterInputNode, FName("Switch Parameter"));

	FMetaSoundBuilderNodeInputHandle onFinishedOutputHandle = builder->AddGraphOutputNode(
		FName("On Finished"),
		FName("Trigger"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle onFinishedOutputNode(onFinishedOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(onFinishedOutputNode, FName("On Finished"));

	// Stage 4: Setup Probability
	setupProbability(builder, switchContainer->PlaybackProbabilityPercent,
		triggerInputHandle, onFinishedOutputHandle, &Layout);

	// Stage 5: Add Core Processing
	int32 numChildren = switchContainer->SoundDictionary.Num();

	if (numChildren < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: No children in switch container %s"), *Name);
		return nullptr;
	}

	// Build child patches recursively and collect case names
	TArray<FChildPatchResult> childResults;
	TArray<FString> caseNames;
	bool bIsStereo = false;

	int32 childIndex = 0;
	for (auto& Pair : switchContainer->SoundDictionary)
	{
		caseNames.Add(Pair.Key);

		FChildPatchResult childResult = BuildChildNode(
			Pair.Value,
			Name,
			childIndex
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Warning, TEXT("BuildSwitchNode: Skipping failed child %s in %s"), *Pair.Key, *Name);
			childIndex++;
			continue;
		}

		childResults.Add(childResult);
		if (childResult.bIsStereo) bIsStereo = true;
		childIndex++;
	}

	if (childResults.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: No valid children built for %s"), *Name);
		return nullptr;
	}

	// Declare audio output handles (populated below)
	FMetaSoundBuilderNodeOutputHandle audioLeftHandle;
	FMetaSoundBuilderNodeOutputHandle audioRightHandle;
	FMetaSoundBuilderNodeOutputHandle onFinishedHandle;

	// CASE 1: Single child - use AudioReroute
	if (childResults.Num() == 1)
	{
		FMetaSoundNodeHandle rerouteHandle = builder->AddNode(AudioRerouteNode, result);
		Layout.RegisterNode(rerouteHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioReroute"));

		// Instantiate child patch as node
		FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[0].Patch, result);
		Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, TEXT("ChildPatch0"));

		// Connect trigger: Play -> Child Play
		FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
		builder->ConnectNodes(triggerInputHandle, childPlayInput, result);
		Layout.RegisterConnection(triggerInputNode, childPatchNode);

		// Connect audio: Child -> Reroute
		FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
			childPatchNode,
			bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
			result
		);
		FMetaSoundBuilderNodeInputHandle rerouteAudioInL = builder->FindNodeInputByName(rerouteHandle, TEXT("InL"), result);
		builder->ConnectNodes(childAudioLeft, rerouteAudioInL, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		if (bIsStereo)
		{
			FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
			FMetaSoundBuilderNodeInputHandle rerouteAudioInR = builder->FindNodeInputByName(rerouteHandle, TEXT("InR"), result);
			builder->ConnectNodes(childAudioRight, rerouteAudioInR, result);
		}

		// Connect finish trigger: Child OnFinished -> Reroute
		FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
		FMetaSoundBuilderNodeInputHandle rerouteExecIn = builder->FindNodeInputByName(rerouteHandle, TEXT("InExec"), result);
		builder->ConnectNodes(childFinished, rerouteExecIn, result);
		Layout.RegisterConnection(childPatchNode, rerouteHandle);

		// Get reroute outputs
		audioLeftHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutL"), result);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutR"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutExec"), result);
	}
	// CASE 2: Multiple children (2-8) - use GisbSwitchNode + Mixer + TriggerAny
	else if (childResults.Num() <= 8)
	{
		// Add GisbSwitchNode for parameter-driven selection
		FMetaSoundNodeHandle switchHandle = builder->AddNode(GisbSwitchNode, result);
		Layout.RegisterNode(switchHandle, EGisbNodeCategory::SignalProcessor, TEXT("SwitchSelector"));

		// Configure GisbSwitchNode
		FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(switchHandle, TEXT("Exec"), result);
		builder->ConnectNodes(triggerInputHandle, execHandle, result);
		Layout.RegisterConnection(triggerInputNode, switchHandle);

		// Connect parameter input to switch node
		FMetaSoundBuilderNodeInputHandle parameterHandle = builder->FindNodeInputByName(switchHandle, TEXT("Parameter Value"), result);
		builder->ConnectNodes(parameterInputHandle, parameterHandle, result);
		Layout.RegisterConnection(parameterInputNode, switchHandle);

		// Set case names (assuming GisbSwitchNode accepts string array)
		FMetaSoundBuilderNodeInputHandle caseNamesHandle = builder->FindNodeInputByName(switchHandle, TEXT("Case Names"), result);
		FAudioParameter caseNamesParam = FAudioParameter(TEXT("Case Names"), caseNames);
		builder->SetNodeInputDefault(caseNamesHandle, FMetasoundFrontendLiteral(caseNamesParam), result);

		// Set default case
		FMetaSoundBuilderNodeInputHandle defaultCaseHandle = builder->FindNodeInputByName(switchHandle, TEXT("Default Case"), result);
		FAudioParameter defaultCaseParam = FAudioParameter(TEXT("Default Case"), switchContainer->DefaultParameterValue.ToString());
		builder->SetNodeInputDefault(defaultCaseHandle, FMetasoundFrontendLiteral(defaultCaseParam), result);

		// Add mixer node (array indexed by numChildren-2)
		FMetaSoundNodeHandle mixerHandle = builder->AddNodeByClassName(
			bIsStereo ? *(*StereoMixerNodes)[childResults.Num() - 2] : *(*MonoMixerNodes)[childResults.Num() - 2],
			result
		);
		Layout.RegisterNode(mixerHandle, EGisbNodeCategory::SignalProcessor, TEXT("AudioMixer"));

		// Add TriggerAny node (first trigger wins)
		FMetaSoundNodeHandle anyHandle = builder->AddNodeByClassName(
			*((*TriggerAnyNodes)[childResults.Num() - 2]),
			result
		);
		Layout.RegisterNode(anyHandle, EGisbNodeCategory::TriggerFlow, TEXT("TriggerAny"));

		// Instantiate child patches and connect
		for (int32 i = 0; i < childResults.Num(); i++)
		{
			FMetaSoundNodeHandle childPatchNode = builder->AddNode(childResults[i].Patch, result);
			FString childNodeName = FString::Printf(TEXT("ChildPatch%d_%s"), i, *caseNames[i]);
			Layout.RegisterNode(childPatchNode, EGisbNodeCategory::SignalSource, childNodeName);

			// Connect Switch output i -> Child Play
			FMetaSoundBuilderNodeOutputHandle switchOut = builder->FindNodeOutputByName(
				switchHandle,
				*FString::Printf(TEXT("Out %d"), i),
				result
			);
			FMetaSoundBuilderNodeInputHandle childPlayInput = builder->FindNodeInputByName(childPatchNode, TEXT("Play"), result);
			builder->ConnectNodes(switchOut, childPlayInput, result);
			Layout.RegisterConnection(switchHandle, childPatchNode);

			// Connect Child audio -> Mixer input i
			FMetaSoundBuilderNodeOutputHandle childAudioLeft = builder->FindNodeOutputByName(
				childPatchNode,
				bIsStereo ? TEXT("Audio Left") : TEXT("Audio Mono"),
				result
			);
			FMetaSoundBuilderNodeInputHandle mixerInLeft = builder->FindNodeInputByName(
				mixerHandle,
				bIsStereo ? *FString::Printf(TEXT("In %d L"), i) : *FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childAudioLeft, mixerInLeft, result);
			Layout.RegisterConnection(childPatchNode, mixerHandle);

			if (bIsStereo)
			{
				FMetaSoundBuilderNodeOutputHandle childAudioRight = builder->FindNodeOutputByName(childPatchNode, TEXT("Audio Right"), result);
				FMetaSoundBuilderNodeInputHandle mixerInRight = builder->FindNodeInputByName(
					mixerHandle,
					*FString::Printf(TEXT("In %d R"), i),
					result
				);
				builder->ConnectNodes(childAudioRight, mixerInRight, result);
			}

			// Connect Child OnFinished -> TriggerAny input i
			FMetaSoundBuilderNodeOutputHandle childFinished = builder->FindNodeOutputByName(childPatchNode, TEXT("On Finished"), result);
			FMetaSoundBuilderNodeInputHandle anyIn = builder->FindNodeInputByName(
				anyHandle,
				*FString::Printf(TEXT("In %d"), i),
				result
			);
			builder->ConnectNodes(childFinished, anyIn, result);
			Layout.RegisterConnection(childPatchNode, anyHandle);
		}

		// Get mixer/any outputs
		audioLeftHandle = builder->FindNodeOutputByName(
			mixerHandle,
			bIsStereo ? TEXT("Out L") : TEXT("Out"),
			result
		);
		if (bIsStereo)
		{
			audioRightHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
		}
		onFinishedHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	}
	else
	{
		// CASE 3: >8 children - hierarchical cascading (TODO)
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: >8 children not yet implemented for %s"), *Name);
		return nullptr;
	}

	// Stage 6: Apply Audio Effects Chain
	if (switchContainer->VolumeDB.randomize || switchContainer->VolumeDB.value != 0)
	{
		ConnectVolume(builder, switchContainer->VolumeDB, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (switchContainer->Pitch.randomize || switchContainer->Pitch.value != 0)
	{
		ConnectPitch(builder, switchContainer->Pitch, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	if (switchContainer->Lowpass.randomize || switchContainer->Lowpass.value != 0)
	{
		ConnectLowpass(builder, switchContainer->Lowpass, bIsStereo,
			&triggerInputHandle, audioLeftHandle, audioRightHandle, &Layout);
	}

	// Stage 7: Connect Graph Outputs
	FMetaSoundBuilderNodeInputHandle audioLeftOutputHandle = builder->AddGraphOutputNode(
		bIsStereo ? FName("Audio Left") : FName("Audio Mono"),
		FName("Audio"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle audioLeftOutputNode(audioLeftOutputHandle.NodeID);
	Layout.RegisterGraphOutputNode(audioLeftOutputNode, bIsStereo ? FName("Audio Left") : FName("Audio Mono"));

	builder->ConnectNodes(audioLeftHandle, audioLeftOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(audioLeftHandle.NodeID), audioLeftOutputNode);

	if (bIsStereo)
	{
		FMetaSoundBuilderNodeInputHandle audioRightOutputHandle = builder->AddGraphOutputNode(
			FName("Audio Right"),
			FName("Audio"),
			FMetasoundFrontendLiteral(),
			result
		);
		FMetaSoundNodeHandle audioRightOutputNode(audioRightOutputHandle.NodeID);
		Layout.RegisterGraphOutputNode(audioRightOutputNode, FName("Audio Right"));

		builder->ConnectNodes(audioRightHandle, audioRightOutputHandle, result);
		Layout.RegisterConnection(FMetaSoundNodeHandle(audioRightHandle.NodeID), audioRightOutputNode);
	}

	builder->ConnectNodes(onFinishedHandle, onFinishedOutputHandle, result);
	Layout.RegisterConnection(FMetaSoundNodeHandle(onFinishedHandle.NodeID), onFinishedOutputNode);

	// Stage 8: Compute Layout & Build Asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(
		builder,
		"ISX - Demute",
		Name,
		PathGlobal,
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildSwitchNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}
