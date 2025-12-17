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

	if (UGisbImportContainerSimpleSound* SimpleSound = Cast<UGisbImportContainerSimpleSound>(ImportContainer))
	{
		BuilderGlobal = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
		PathGlobal = "/Game/A_TESTING/Metasounds";
		SetupNodes();

		UGISB_NewMetasoundBuilder::BuildSimpleSoundNode(SimpleSound, dataAsset->EventName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Container is not a SimpleSound type - type is: %s"),
			   *ImportContainer->GetClass()->GetName());
	}

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

	// Determine if stereo
	bool isStereo = simpleSound->SoundWave && simpleSound->SoundWave->NumChannels > 1;

	// Add Graph INPUT nodes (these become the patch's inputs when used as a node)
	FMetaSoundBuilderNodeOutputHandle triggerInputHandle = builder->AddGraphInputNode(
	  FName("Play"),
	  FName("Trigger"),
	  FMetasoundFrontendLiteral(),
	  result
	);

	// Add Graph OUTPUT nodes (these become the patch's outputs when used as a node)
	FMetaSoundBuilderNodeInputHandle onFinishedOutputHandle = builder->AddGraphOutputNode(
	  FName("On Finished"),
	  FName("Trigger"),
	  FMetasoundFrontendLiteral(),
	  result
	);
	
	setupProbability(builder, simpleSound->PlaybackProbabilityPercent, triggerInputHandle, onFinishedOutputHandle);

	//ConnectProbabilityNode(builder, simpleSound->PlaybackProbabilityPercent, triggerInputHandle, onFinishedOutputHandle);

	// Add Wave Player node
	FMetaSoundNodeHandle wavePlayerHandle = builder->AddNodeByClassName(
	  isStereo ? *WavePlayerStereoNode : *WavePlayerMonoNode,
	  result
	);

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
	setupAttributes(builder, simpleSound, isStereo, &triggerInputHandle, audioLeftHandle, audioRightHandle);

	FMetaSoundBuilderNodeInputHandle audioLeftOutputHandle = builder->AddGraphOutputNode(
	  isStereo ? FName("Audio Left") : FName("Audio Mono"),
	  FName("Audio"),
	  FMetasoundFrontendLiteral(),
	  result
	);

	// Connect to outputs
	builder->ConnectNodes(onFinishedHandle, onFinishedOutputHandle, result);
	builder->ConnectNodes(audioLeftHandle, audioLeftOutputHandle, result);

	if (isStereo)
	{
	  FMetaSoundBuilderNodeInputHandle audioRightOutputHandle = builder->AddGraphOutputNode(
	      FName("Audio Right"),
	      FName("Audio"),
	      FMetasoundFrontendLiteral(),
	      result
	  );
	  builder->ConnectNodes(audioRightHandle, audioRightOutputHandle, result);
	}

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


void UGISB_NewMetasoundBuilder::setupProbability(UMetaSoundPatchBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& playTrigger, FMetaSoundBuilderNodeInputHandle& onFinished)
{
	if (probability > 99.99f)
	{
		return;
	}

	ConnectProbabilityNode(builder, probability, playTrigger, onFinished);
	
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

void UGISB_NewMetasoundBuilder::ConnectProbabilityNode(UMetaSoundPatchBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& executionHandle, FMetaSoundBuilderNodeInputHandle& finishHandle)
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

	// Update finishHandle to point to Trigger Any Input 0 (for success path)
	FMetaSoundBuilderNodeInputHandle succesHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 0"), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectProbabilityNode: FAILED to find Trigger Any In 0 input"));
		return;
	}

	finishHandle = succesHandle;
}

void UGISB_NewMetasoundBuilder::ConnectVolume(UMetaSoundPatchBuilder* builder, const FGisbVolume volume, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	EMetaSoundBuilderResult result;

	// Add Volume node
	FMetaSoundNodeHandle volumeHandle = builder->AddNode(GisbVolumeNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !volumeHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to add Volume node"));
		return;
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
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to connect left audio input"));
		return;
	}

	if (isStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectVolume: FAILED to connect right audio input"));
			return;
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

void UGISB_NewMetasoundBuilder::ConnectPitch(UMetaSoundPatchBuilder* builder, const FGisbPitch pitch, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	EMetaSoundBuilderResult result;

	// Add Pitch node
	FMetaSoundNodeHandle pitchHandle = builder->AddNode(GisbPitchNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !pitchHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to add Pitch node"));
		return;
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
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to connect left audio input"));
		return;
	}

	if (isStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectPitch: FAILED to connect right audio input"));
			return;
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

void UGISB_NewMetasoundBuilder::ConnectLowpass(UMetaSoundPatchBuilder* builder, const FGisbLowPass lowpass, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	EMetaSoundBuilderResult result;

	// Add Lowpass node
	FMetaSoundNodeHandle lowpassHandle = builder->AddNode(GisbLowpassNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded || !lowpassHandle.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to add Lowpass node"));
		return;
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
	}

	// Connect audio inputs
	builder->ConnectNodes(firstAudioHandle, inLeftHandle, result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to connect left audio input"));
		return;
	}

	if (isStereo)
	{
		builder->ConnectNodes(secondAudioHandle, inRightHandle, result);
		if (result != EMetaSoundBuilderResult::Succeeded)
		{
			UE_LOG(LogTemp, Error, TEXT("ConnectLowpass: FAILED to connect right audio input"));
			return;
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

void UGISB_NewMetasoundBuilder::setupAttributes(UMetaSoundPatchBuilder* builder, UGisbImportContainerSimpleSound* simpleSound, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	// Apply Volume effect if configured
	if (simpleSound->VolumeDB.randomize || simpleSound->VolumeDB.value != 0)
	{
		ConnectVolume(builder, simpleSound->VolumeDB, isStereo, executionHandle, firstAudioHandle, secondAudioHandle);
	}

	// Apply Pitch effect if configured
	if (simpleSound->Pitch.randomize || simpleSound->Pitch.value != 0)
	{
		ConnectPitch(builder, simpleSound->Pitch, isStereo, executionHandle, firstAudioHandle, secondAudioHandle);
	}

	// Apply Lowpass effect if configured
	if (simpleSound->Lowpass.randomize || simpleSound->Lowpass.value != 0)
	{
		ConnectLowpass(builder, simpleSound->Lowpass, isStereo, executionHandle, firstAudioHandle, secondAudioHandle);
	}
}
